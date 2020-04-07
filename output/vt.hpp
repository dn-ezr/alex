#ifndef __vt__
#define __vt__

namespace alioth{
    constexpr int ANY = 2;
    constexpr int ASSUME = 4;
    constexpr int ASYNC = 5;
    constexpr int ATOMIC = 6;
    constexpr int AWAIT = 7;
    constexpr int BREAK = 8;
    constexpr int CLASS = 9;
    constexpr int CONST = 10;
    constexpr int CONTINUE = 11;
    constexpr int ELSE = 13;
    constexpr int ENUM = 14;
    constexpr int FOR = 15;
    constexpr int IF = 16;
    constexpr int INTERFACE = 18;
    constexpr int MODULE = 19;
    constexpr int OPERATOR = 23;
    constexpr int PUBLIC = 24;
    constexpr int PROTECTED = 25;
    constexpr int PRIVATE = 26;
    constexpr int RETURN = 27;
    constexpr int STATIC = 28;
    constexpr int THIS = 29;
    constexpr int VAR = 30;
    constexpr int WHILE = 31;
    namespace COMMENT{
        constexpr int BLOCK = 32;
        constexpr int LINE = 33;
    }
    namespace L{
        constexpr int LABEL = 34;
        namespace I{
            constexpr int H = 36;
            constexpr int B = 37;
            constexpr int N = 38;
        }
    }
    namespace OP{
        constexpr int AND = 1;
        constexpr int AS = 3;
        constexpr int DELETE = 12;
        constexpr int IN = 17;
        constexpr int NEW = 20;
        constexpr int NOT = 21;
        constexpr int OR = 22;
    }
}

#endif