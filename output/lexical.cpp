
#ifndef __lexical_cpp__
#define __lexical_cpp__

#include "vt.hpp"
#include "token.hpp"
#include "lexical.hpp"

namespace alioth {

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
            
            case 1103: 
                if( '0' <= m_pre and m_pre <= '9' ) { _into(1103); break; }
                { _goto(1101); break; }
            break;
            case 1087: 
                if( ( 48 <= m_pre and m_pre <= 49 ) ){ _into(1090); break; }
                accept(1, 0, true);
            break;
            case 1090: 
                if( ( 48 <= m_pre and m_pre <= 49 ) ){ _into(1090); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(1096); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(1096); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(1096); break; }
                { accept(1, 37, false); break; }
            break;
            case 1027: 
                if( ( 48 <= m_pre and m_pre <= 57 ) or ( 65 <= m_pre and m_pre <= 70 ) or ( 97 <= m_pre and m_pre <= 102 ) ){ _into(1030); break; }
                accept(1, 0, true);
            break;
            case 1030: 
                if( ( 48 <= m_pre and m_pre <= 57 ) or ( 65 <= m_pre and m_pre <= 70 ) or ( 97 <= m_pre and m_pre <= 102 ) ){ _into(1030); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(1082); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(1082); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(1082); break; }
                { accept(1, 36, false); break; }
            break;
            case 136: 
                if( 10 == m_pre ){ _into(137); break; }
                if( 1 <= m_pre and m_pre <= 127 ) { _into(136); break; }
                accept(1, 0, true);
            break;
            case 100: 
                if( 100 == m_pre ){ _into(101); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 9: 
                if( 101 == m_pre ){ _into(10); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 99: 
                if( 101 == m_pre ){ _into(100); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 105: 
                if( 101 == m_pre ){ _into(106); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 107: 
                if( 101 == m_pre ){ _into(108); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 129: 
                if( 101 == m_pre ){ _into(130); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 119: 
                if( 104 == m_pre ){ _into(120); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 126: 
                if( 104 == m_pre ){ _into(127); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 94: 
                if( 105 == m_pre ){ _into(102); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 111 == m_pre ){ _into(95); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 116: 
                if( 105 == m_pre ){ _into(117); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 120: 
                if( 105 == m_pre ){ _into(121); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 127: 
                if( 105 == m_pre ){ _into(128); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 128: 
                if( 108 == m_pre ){ _into(129); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 111: 
                if( 110 == m_pre ){ _into(112); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 11: 
                if( 110 == m_pre ){ _into(12); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 110: 
                if( 114 == m_pre ){ _into(111); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 124: 
                if( 114 == m_pre ){ _into(125); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 121: 
                if( 115 == m_pre ){ _into(122); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 104: 
                if( 116 == m_pre ){ _into(105); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 108: 
                if( 116 == m_pre ){ _into(109); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 113: 
                if( 116 == m_pre ){ _into(114); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 115: 
                if( 116 == m_pre ){ _into(116); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 2: 
                if( 116 == m_pre ){ _into(14); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 119 == m_pre ){ _into(19); break; }
                if( 110 == m_pre ){ _into(3); break; }
                if( 115 == m_pre ){ _into(6); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 109: 
                if( 117 == m_pre ){ _into(110); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 102: 
                if( 118 == m_pre ){ _into(103); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 6: 
                if( 121 == m_pre ){ _into(11); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 115 == m_pre ){ _into(7); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 1101: 
                if( 36 == m_pre or 95 == m_pre ){ _into(1106); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(1106); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(1106); break; }
                { accept(1, 38, false); break; }
            break;
            case 139: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 4: case 5: case 10: case 13: case 18: case 22: case 27: case 32: case 36: case 41: case 47: case 51: case 54: case 57: case 59: case 67: case 73: case 76: case 78: case 80: case 87: case 93: case 101: case 106: case 112: case 118: case 122: case 125: case 130: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 144: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { accept(1, 34, false); break; }
            break;
            case 3: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 100 == m_pre ){ _into(4); break; }
                if( 121 == m_pre ){ _into(5); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 69: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 100 == m_pre ){ _into(70); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 24: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(25); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 40: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(41); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 42: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(43); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 44: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(45); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 46: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(47); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 50: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(51); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 61: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(62); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 66: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(67); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 72: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(73); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 74: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(75); break; }
                if( 111 == m_pre ){ _into(77); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 81: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(82); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 96: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 101 == m_pre ){ _into(97); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 58: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 102 == m_pre ){ _into(59); break; }
                if( 110 == m_pre ){ _into(60); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 63: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 102 == m_pre ){ _into(64); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 16: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 105 == m_pre ){ _into(17); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 20: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 105 == m_pre ){ _into(21); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 37: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 105 == m_pre ){ _into(38); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 91: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 105 == m_pre ){ _into(92); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 26: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 107 == m_pre ){ _into(27); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 28: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 108 == m_pre ){ _into(29); break; }
                if( 111 == m_pre ){ _into(33); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 43: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 108 == m_pre ){ _into(44); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 48: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 108 == m_pre ){ _into(49); break; }
                if( 110 == m_pre ){ _into(52); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 71: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 108 == m_pre ){ _into(72); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 90: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 108 == m_pre ){ _into(91); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 15: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 109 == m_pre ){ _into(16); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 53: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 109 == m_pre ){ _into(54); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 8: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 109 == m_pre ){ _into(9); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 33: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 110 == m_pre ){ _into(34); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 38: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 110 == m_pre ){ _into(39); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 14: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 111 == m_pre ){ _into(15); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 55: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 111 == m_pre ){ _into(56); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 68: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 111 == m_pre ){ _into(69); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 85: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 111 == m_pre ){ _into(86); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 23: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(24); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 56: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(57); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 62: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(63); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 79: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(80); break; }
                if( 112 == m_pre ){ _into(81); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 82: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(83); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 86: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 114 == m_pre ){ _into(87); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 30: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 115 == m_pre ){ _into(31); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 31: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 115 == m_pre ){ _into(32); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 34: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 115 == m_pre ){ _into(35); break; }
                if( 116 == m_pre ){ _into(37); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 49: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 115 == m_pre ){ _into(50); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 21: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(22); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 35: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(36); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 45: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(46); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 60: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(61); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 77: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(78); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 84: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(85); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 95: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(96); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 98: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 116 == m_pre ){ _into(99); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 39: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 117 == m_pre ){ _into(40); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 52: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 117 == m_pre ){ _into(53); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 70: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 117 == m_pre ){ _into(71); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 7: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 117 == m_pre ){ _into(8); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 88: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 117 == m_pre ){ _into(89); break; }
                if( 114 == m_pre ){ _into(94); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 75: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 119 == m_pre ){ _into(76); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 19: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 97 == m_pre ){ _into(20); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 25: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 97 == m_pre ){ _into(26); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 29: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 97 == m_pre ){ _into(30); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 64: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 97 == m_pre ){ _into(65); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 83: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 97 == m_pre ){ _into(84); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 89: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 98 == m_pre ){ _into(90); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 17: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 99 == m_pre ){ _into(18); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 65: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 99 == m_pre ){ _into(66); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 92: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 99 == m_pre ){ _into(93); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 97: 
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( 99 == m_pre ){ _into(98); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 131: 
                if( 42 == m_pre ){ _into(132); break; }
                if( 47 == m_pre ){ _into(136); break; }
                accept(1, 0, true);
            break;
            case 132: 
                if( 42 == m_pre ){ _into(134); break; }
                if( 1 <= m_pre and m_pre <= 127 ) { _into(132); break; }
                accept(1, 0, true);
            break;
            case 134: 
                if( 47 == m_pre ){ _into(135); break; }
                { _goto(132); break; }
            break;
            case 1: 
                if( 48 == m_pre ){ _into(1026); break; }
                if( 114 == m_pre ){ _into(107); break; }
                if( 115 == m_pre ){ _into(113); break; }
                if( 116 == m_pre ){ _into(119); break; }
                if( 118 == m_pre ){ _into(123); break; }
                if( 119 == m_pre ){ _into(126); break; }
                if( 47 == m_pre ){ _into(131); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(139); break; }
                if( 97 == m_pre ){ _into(2); break; }
                if( 98 == m_pre ){ _into(23); break; }
                if( 99 == m_pre ){ _into(28); break; }
                if( 100 == m_pre ){ _into(42); break; }
                if( 101 == m_pre ){ _into(48); break; }
                if( 102 == m_pre ){ _into(55); break; }
                if( 105 == m_pre ){ _into(58); break; }
                if( 109 == m_pre ){ _into(68); break; }
                if( 110 == m_pre ){ _into(74); break; }
                if( 111 == m_pre ){ _into(79); break; }
                if( 112 == m_pre ){ _into(88); break; }
                if( -1 == m_pre ){ m_state = 0; break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(1103); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(139); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(139); break; }
                accept(1, 0, true);
            break;
            case 1026: 
                if( 48 == m_pre ){ _into(1026); break; }
                if( 88 == m_pre or 120 == m_pre ){ _into(1027); break; }
                if( 66 == m_pre or 98 == m_pre ){ _into(1087); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(1103); break; }
                { _goto(1103); break; }
            break;
            case 103: 
                if( 97 == m_pre ){ _into(104); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 114: 
                if( 97 == m_pre ){ _into(115); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 123: 
                if( 97 == m_pre ){ _into(124); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                accept(1, 0, true);
            break;
            case 117: 
                if( 99 == m_pre ){ _into(118); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 12: 
                if( 99 == m_pre ){ _into(13); break; }
                if( 36 == m_pre or 95 == m_pre ){ _into(144); break; }
                if( '0' <= m_pre and m_pre <= '9' ) { _into(144); break; }
                if( 'A' <= m_pre and m_pre <= 'Z' ) { _into(144); break; }
                if( 'a' <= m_pre and m_pre <= 'z' ) { _into(144); break; }
                { _goto(144); break; }
            break;
            case 1082: 
                { accept(1, -36, false); break; }
            break;
            case 1096: 
                { accept(1, -37, false); break; }
            break;
            case 1106: 
                { accept(1, -38, false); break; }
            break;
            case 135: 
                { accept(1, 32, false); break; }
            break;
            case 137: 
                { accept(1, 33, false); break; }
            break;
            default: throw std::runtime_error("LexicalContext::perform(): Impossible state used"); break;
        }
    }

    return m_offset;
}

}

#endif
