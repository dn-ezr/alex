#ifndef __main_cpp__
#define __main_cpp__

#include <fstream>
#include "alex.hpp"

int main( int argc, char **argv ) {
    if( argc < 2 ) return 1;
    auto is = std::ifstream(argv[1]);
    auto lex = alex::lex::compile(is);
    auto files = lex.gencpp("alioth");
    for( auto [fname, content] : files ) {
        auto os = std::ofstream("output/"+fname);
        os << content;
    }
    return 0;
}

#endif