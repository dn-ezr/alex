#ifndef __main_cpp__
#define __main_cpp__

#include <fstream>
#include "alex.hpp"

int main( int argc, char **argv ) {
    if( argc < 3 ) return 1;
    auto is = std::ifstream(argv[1]);
    auto diagram = alex::diagram::compile( is );
    auto input = std::ifstream(argv[2]);

    auto context = alex::context(diagram);
    std::cout << "--------[PROGRAM]--------" << std::endl;
    diagram.store(std::cout);
    std::cout << "--------[OUTPUT]--------" << std::endl;
    auto tokens = context.perform( input, std::cout );
    std::cout << "--------[TOKENS]--------" << std::endl;
    for( auto [id,str] : tokens ) {
        std::cout << "( " << id << ", " << str << " )" << std::endl;
    }
    return 0;
}

#endif