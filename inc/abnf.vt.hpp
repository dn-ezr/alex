#ifndef __vt__
#define __vt__

namespace ABNF::VT {
    constexpr int $ = 0;
    constexpr int LABEL = 2;
    constexpr int SPACE = 3;
    constexpr int ASSIGN = 4;
    constexpr int STRING = 5;
    constexpr int COMMA = 6;
    constexpr int SEMI = 7;
    constexpr int COLON = 8;
    constexpr int QM = 9;
    constexpr int EM = 10;
    constexpr int GT = 11;
    constexpr int DOT = 12;
    constexpr int AT = 13;
    constexpr int OR = 14;
    constexpr int STAR = 15;
    constexpr int COMMENT = 22;
    namespace C {
        constexpr int SEQ = 19;
        constexpr int OPT = 20;
        constexpr int ANY = 21;
    }
    namespace O {
        constexpr int SEQ = 16;
        constexpr int OPT = 17;
        constexpr int ANY = 18;
    }
}

#endif