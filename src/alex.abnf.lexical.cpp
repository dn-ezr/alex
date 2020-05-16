
#ifndef __lexical_cpp__
#define __lexical_cpp__

#include "abnf.vt.hpp"
#include "alex.hpp"
#include "abnf.lexical.hpp"

namespace ABNF {

void LexicalContext::_goto( int state ) {
    m_state = state;
}

void LexicalContext::_into( int state ) {
    m_cur.el = m_line;
    m_cur.ec = m_column;
    m_cur.tx += m_pre;
    m_offset += 1;
    if( m_pre == '\n' ) m_line += m_column = 1;
    else m_column += 1;
    m_is->get();
    m_state = state;
}

void LexicalContext::accept( int state, int id, bool take ) {
    if( take ) _into(state);
    else _goto(state);
    m_cur.id = id;
    if( ontoken ) 
        if( !ontoken(std::move(m_cur)) ) m_state = 0;
    m_cur.id = 0;
    m_cur.bl = m_cur.el = m_line;
    m_cur.bc = m_cur.ec = m_column;
    m_cur.tx.clear();
}

int LexicalContext::perform( std::istream& source ) {

    if( !source.good() ) return 0;
    m_is = &source;

    m_cur.bl = m_cur.bc = m_cur.el = m_cur.ec = m_state = m_line = m_column = 1;
    m_offset = 0;

    while( m_state > 0 ) {
        m_pre = source.peek();
        
        switch( m_state ) {
            
            case 23: 
                if( '*' == m_pre ){ _into(24); break; }
                accept(-1, 0, false);
            break;
            case 24: 
                if( '*' == m_pre ){ _into(26); break; }
                if( 1 <= m_pre and m_pre <= 255 ) { _into(24); break; }
                accept(-1, 0, false);
            break;
            case 26: 
                if( '/' == m_pre ){ _into(25); break; }
                if( 0 < m_pre ) { _goto(24); break; }
                accept(-1, 0, false);
            break;
            case 2: 
                if( ':' == m_pre or '_' == m_pre ){ _into(2); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(2); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(2); break; }
                if( 0 < m_pre ) { accept(1, VT::LABEL, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::LABEL, false); break; }
                accept(-1, 0, false);
            break;
            case 1: 
                if( '?' == m_pre ){ _into(10); break; }
                if( '!' == m_pre ){ _into(11); break; }
                if( '>' == m_pre ){ _into(12); break; }
                if( '.' == m_pre ){ _into(13); break; }
                if( '@' == m_pre ){ _into(14); break; }
                if( '|' == m_pre ){ _into(15); break; }
                if( '*' == m_pre ){ _into(16); break; }
                if( '(' == m_pre ){ _into(17); break; }
                if( '[' == m_pre ){ _into(18); break; }
                if( '{' == m_pre ){ _into(19); break; }
                if( ':' == m_pre or '_' == m_pre ){ _into(2); break; }
                if( ')' == m_pre ){ _into(20); break; }
                if( ']' == m_pre ){ _into(21); break; }
                if( '}' == m_pre ){ _into(22); break; }
                if( '/' == m_pre ){ _into(23); break; }
                if( '=' == m_pre ){ _into(4); break; }
                if( '\"' == m_pre ){ _into(5); break; }
                if( ',' == m_pre ){ _into(8); break; }
                if( ';' == m_pre ){ _into(9); break; }
                if( m_pre == ' ' or m_pre == '\t' or m_pre == '\n' or m_pre == '\r' ) { _into(3); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(2); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(2); break; }
                if( 0 == m_pre or -1 == m_pre ) { m_state = 0; break; }
                accept(-1, 0, false);
            break;
            case 5: 
                if( '\\' == m_pre ){ _into(6); break; }
                if( '\"' == m_pre ){ _into(7); break; }
                if( 1 <= m_pre and m_pre <= 255 ) { _into(5); break; }
                accept(-1, 0, false);
            break;
            case 4: 
                if( 0 < m_pre ) { accept(1, VT::ASSIGN, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::ASSIGN, false); break; }
                accept(-1, 0, false);
            break;
            case 14: 
                if( 0 < m_pre ) { accept(1, VT::AT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::AT, false); break; }
                accept(-1, 0, false);
            break;
            case 22: 
                if( 0 < m_pre ) { accept(1, VT::C::ANY, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::C::ANY, false); break; }
                accept(-1, 0, false);
            break;
            case 21: 
                if( 0 < m_pre ) { accept(1, VT::C::OPT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::C::OPT, false); break; }
                accept(-1, 0, false);
            break;
            case 20: 
                if( 0 < m_pre ) { accept(1, VT::C::SEQ, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::C::SEQ, false); break; }
                accept(-1, 0, false);
            break;
            case 8: 
                if( 0 < m_pre ) { accept(1, VT::COMMA, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::COMMA, false); break; }
                accept(-1, 0, false);
            break;
            case 25: 
                if( 0 < m_pre ) { accept(1, VT::COMMENT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::COMMENT, false); break; }
                accept(-1, 0, false);
            break;
            case 13: 
                if( 0 < m_pre ) { accept(1, VT::DOT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::DOT, false); break; }
                accept(-1, 0, false);
            break;
            case 11: 
                if( 0 < m_pre ) { accept(1, VT::EM, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::EM, false); break; }
                accept(-1, 0, false);
            break;
            case 12: 
                if( 0 < m_pre ) { accept(1, VT::GT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::GT, false); break; }
                accept(-1, 0, false);
            break;
            case 19: 
                if( 0 < m_pre ) { accept(1, VT::O::ANY, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::O::ANY, false); break; }
                accept(-1, 0, false);
            break;
            case 18: 
                if( 0 < m_pre ) { accept(1, VT::O::OPT, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::O::OPT, false); break; }
                accept(-1, 0, false);
            break;
            case 17: 
                if( 0 < m_pre ) { accept(1, VT::O::SEQ, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::O::SEQ, false); break; }
                accept(-1, 0, false);
            break;
            case 15: 
                if( 0 < m_pre ) { accept(1, VT::OR, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::OR, false); break; }
                accept(-1, 0, false);
            break;
            case 10: 
                if( 0 < m_pre ) { accept(1, VT::QM, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::QM, false); break; }
                accept(-1, 0, false);
            break;
            case 9: 
                if( 0 < m_pre ) { accept(1, VT::SEMI, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::SEMI, false); break; }
                accept(-1, 0, false);
            break;
            case 16: 
                if( 0 < m_pre ) { accept(1, VT::STAR, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::STAR, false); break; }
                accept(-1, 0, false);
            break;
            case 7: 
                if( 0 < m_pre ) { accept(1, VT::STRING, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::STRING, false); break; }
                accept(-1, 0, false);
            break;
            case 6: 
                if( 1 <= m_pre and m_pre <= 255 ) { _into(5); break; }
                accept(-1, 0, false);
            break;
            case 3: 
                if( m_pre == ' ' or m_pre == '\t' or m_pre == '\n' or m_pre == '\r' ) { _into(3); break; }
                if( 0 < m_pre ) { accept(1, VT::SPACE, false); break; }
                if( 0 == m_pre or -1 == m_pre ) { accept(1, VT::SPACE, false); break; }
                accept(-1, 0, false);
            break;
            default: throw std::runtime_error("LexicalContext::perform(): Impossible state used"); break;
        }
    }

    m_is = nullptr;
    return m_offset;
}

}

#endif
