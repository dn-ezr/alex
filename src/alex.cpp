#ifndef __alex_cpp__
#define __alex_cpp__

#include "alex.internal.hpp"

namespace alex {

std::vector<std::string> split( const std::string& str, const char spliter ) {
    std::vector<std::string> ret;
    std::string it;
    for( auto c : str ) {
        if( c == spliter ) {
            ret.push_back(it);
            it.clear();
        } else {
            it += c;
        }
    }
    ret.push_back(it);
    return ret;
}

std::string operator * ( const std::string& str, int factor ) {
    std::string s;
    while( factor-- > 0 ) s += str;
    return s;
}

}

#endif