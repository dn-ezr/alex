#ifndef __main_cpp__
#define __main_cpp__

#include <fstream>
#include "alex.hpp"

int main( int argc, char **argv ) {
    if( argc < 2 ) return 1;
    std::string input;
    std::string lang;
    std::set<std::string> generate;
    std::string to_path;
    for( auto i = 1; argv[i] != 0; i++ ) {
        if( !strcmp("-l", argv[i]) ) {
            if( lang.size() ) {
                std::cout << "The language name has been specified already" << std::endl;
                return 1;
            }
            if( !argv[++i] ) {
                std::cout << "Argument missing for command -l" << std::endl;
                return 1;
            }
            lang = std::string(argv[i]);
        } else if( !strcmp("-t", argv[i]) ) {
            if( to_path.size() ) {
                std::cout << "The target path has been specified already" << std::endl;
                return 1;
            }
            if( !argv[++i] ) {
                std::cout << "Argument missing for command -t" << std::endl;
                return 1;
            }
            to_path = std::string(argv[i]);
        } else if( !strcmp("-g", argv[i]) ) {
            if( !argv[++i] ) {
                std::cout << "Argument missing for command -g" << std::endl;
                return 1;
            }
            std::string temp = argv[i];
            if( temp != "lexical.cpp" and temp != "lexical.hpp" and temp != "token.hpp" and temp != "vt.hpp" ) {
                std::cout << "unavailable generate target" << std::endl;
                return 1;
            }
            generate.insert(temp);
        } else if( !strcmp("-h", argv[i]) ) {
            std::cout << R"...(alex : generate lexical parser from rule definitions
Usage: alex [OPTIONS] input_file
    -l <lang> specify the language name
    -t <path> specify the output path, or the output will flow to stdio
    -g <name> specify which output you need, this command can be used multiple times
        available names as follow:
            lexical.cpp
            lexical.hpp
            token.hpp
            vt.hpp
        if you don't specify any of then, defaulty I will print the Alex DFA on standard output stream
    -h show this help page
)...";
            return 0;
        } else {
            if( !input.empty() ) {
                std::cout << "The lexical input has been specified already" << std::endl;
                return 1;
            }
            input = std::string(argv[i]);
        }
    }
    if( input.empty() ) {
        std::cout << "No input specified" << std::endl;
        return 1;
    }
    if( lang.empty() ) {
        std::cout << "No language name specified " << std::endl;
        return 1;
    }
    auto is = std::ifstream(input);
    if( !is.good() ) {
        std::cout << "Cannot open the input file for read" << std::endl;
        return 1;
    }
    auto lex = alex::lex::compile(is);
    if( generate.empty() ) {
        auto dfa = lex.compile();
        std::cout << dfa << std::endl;
    } else {
        for( auto& fname : generate ) {
            std::string gen;
            if( fname == "lexical.hpp" ) gen = lex.genctxd(lang);
            else if( fname == "lexical.cpp" ) gen = lex.genctxi(lang);
            else if( fname == "token.hpp" ) gen = lex.gentokend(lang);
            else if( fname == "vt.hpp" ) gen = lex.genvtd(lang);
            if( to_path.empty() ) {
                std::cout << gen << std::endl;
            } else {
                auto os = std::ofstream(to_path+fname);
                if( os.good() ) os << gen;
                else std::cout << "failed generating file " << (to_path+fname) << std::endl;
            }
        }
    }
    return 0;
}

#endif