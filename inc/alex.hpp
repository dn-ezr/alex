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

using instruction = std::tuple<int,const chainz<json>>;
using program = chainz<instruction>;
using rules = std::map<int, program>;
using token = std::tuple<int,std::string>;
using tokens = chainz<token>;

class diagram : public std::map<int, rules> {

    public:
        static diagram compile( std::istream& );
        static diagram load( std::istream& );

        bool store( std::ostream& );
};

struct vstack {
    json& a,b,c,d;
    char*& mem;
    const int pre;
    int& target_state;
    std::string& buf;
    tokens& results;
    std::ostream& os;
    bool perform;
    bool input;
    bool skip;
};

class context {

    private:
        const rules* state = nullptr;
        int target_state = 0;
        const diagram& diag;

        json a,b,c,d;
        char* mem = nullptr;

        int pre = -1;
        int line, column;
        std::string buf;

    private:
        bool initiate();

    public:
        context( const alex::diagram& diagram );
        
        tokens perform( std::istream&, std::ostream& );
};

}

#endif