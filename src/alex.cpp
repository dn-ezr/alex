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

std::string replace( const std::string& str, const std::map<std::string,std::string>& job ) {
    std::string ret;
    size_t off = 0;
    while( true ) {
        std::string key;
        size_t cut = std::string::npos;
        for( auto& [k,_] : job ) {
            auto c = str.find(k, off );
            if( c != std::string::npos and (cut == std::string::npos or c < cut) ) {
                cut = c;
                key = k;
            }
        }
        if( cut != std::string::npos ) {
            ret += str.substr( off, cut-off ) + job.at(key);
            off = cut + key.size();
        } else {
            ret += str.substr( off );
            break;
        }
    }
    return ret;
}

std::string operator * ( const std::string& str, int factor ) {
    std::string s;
    while( factor-- > 0 ) s += str;
    return s;
}

}

#endif