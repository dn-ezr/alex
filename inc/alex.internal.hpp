#ifndef __alex_internal__
#define __alex_internal__

#include <functional>
#include <string>
#include <map>
#include "jsonz.hpp"
#include "alex.hpp"

namespace alex {

using fsm_instruction = std::tuple<fsm::cmd_t,chainz<json>>;
using fsm_program = chainz<fsm_instruction>;
using fsm_state = std::map<int, fsm_program>;

template<typename T>
std::vector<T> range( std::vector<T>& total ) {
    std::vector<T> ret;
    int take = 0;
    for( auto item : total ) {
        if( take == 0 or total[take-1] + 1 == item ) take += 1;
        else break;
    }
    ret.insert(ret.begin(),total.begin(), total.begin()+take);
    total.erase(total.begin(), total.begin() + take);
    return ret;
}

struct ns {
    std::map<std::string,struct ns> sub;
    std::map<int,std::string> def;
};

extern const std::string code_lexical_hpp;
extern const std::string code_token_hpp;
extern const std::string code_lexical_cpp;

}

#endif