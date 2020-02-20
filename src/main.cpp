#ifndef __main_cpp__
#define __main_cpp__

#include <fstream>
#include "alex.hpp"

int main( int argc, char **argv ) {
    if( argc < 2 ) return 1;
    auto is = std::ifstream(argv[1]);
    auto lex = alex::lexical_rules::compile(is);
    auto diagram = lex.compile();
    std::cout << "[--------FSM--------]" << std::endl;
    std::cout << diagram;
    std::cout << "[--------LEXICAL--------]" << std::endl;
    for( auto [id,rule] : lex )
        std::cout << id << ' ' << rule.name << " = /" << rule.match << "/" << rule.suffix << "/" << std::endl;
}

#endif