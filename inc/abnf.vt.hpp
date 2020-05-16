#ifndef __vt__
#define __vt__

namespace ABNF::VT {
    constexpr int $ = 0;
    constexpr int NAME = 2;
    constexpr int SPACE = 3;
    constexpr int ASSIGN = 4;
    constexpr int STRING = 5;
    constexpr int COMMA = 6;
    constexpr int SEMI = 7;
    constexpr int COLON = 8;
    constexpr int QM = 9;
    constexpr int EM = 10;
    constexpr int DOT = 11;
    constexpr int AT = 12;
    constexpr int OR = 13;
    constexpr int STAR = 14;
    constexpr int COMMENT = 21;
    namespace C {
        constexpr int SEQ = 18;
        constexpr int OPT = 19;
        constexpr int ANY = 20;
    }
    namespace O {
        constexpr int SEQ = 15;
        constexpr int OPT = 16;
        constexpr int ANY = 17;
    }
}

#endif