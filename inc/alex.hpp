#ifndef __alex__
#define __alex__

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <list>
#include "jsonz.hpp"

namespace alex {

using instruction = std::tuple<int,chainz<json>>;
using program = chainz<instruction>;
using state = std::map<int, program>;
using diagram = std::map<int, state>;

using token = std::tuple<int,std::string>;
using tokens = chainz<token>;

diagram compile( std::istream& );
diagram load( std::istream& );
bool store( const diagram&, std::ostream& );

class context {

    public:
        const diagram& diagram;
        json var[4];
        char* mem = nullptr;
        int pre = -1;
        int target_state = 0;
        state* current_state = nullptr;

    public:
        context( const alex::diagram& diagram );
};

}

#endif