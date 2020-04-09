#ifndef __code_lexical_cpp_cpp__
#define __code_lexical_cpp_cpp__

#include "alex.internal.hpp"

namespace alex {

const std::string code_lexical_cpp = R"...(
#ifndef __lexical_cpp__
#define __lexical_cpp__

#include "vt.hpp"
#include "token.hpp"
#include "lexical.hpp"

namespace %l {

void LexicalContext::_goto( int state ) {
    m_state = state;
}

void LexicalContext::_into( int state ) {
    m_cur.tx += m_pre;
    m_offset += 1;
    if( m_pre == '\n' ) m_line += m_column = 1;
    else m_column += 1;
    m_state = state;
}

void LexicalContext::accept( int state, int id, bool take ) {
    if( take ) _into(state);
    else _goto(state);
    m_cur.id = id;
    if( ontoken ) 
        if( !ontoken(std::move(m_cur)) ) m_state = 0;
    m_cur.id = m_cur.bl = m_cur.bc = m_cur.el = m_cur.ec = 0;
    m_cur.tx.clear();
}

int LexicalContext::perform( std::istream& source ) {

    if( !source.good() ) return 0;

    m_state = m_line = m_column = 1;
    m_offset = 0;

    while( m_state > 0 ) {
        m_pre = source.peek();
        
        switch( m_state ) {
            %dfa
            default: throw std::runtime_error("LexicalContext::perform(): Impossible state used"); break;
        }
    }

    return m_offset;
}

}

#endif
)...";

}

#endif