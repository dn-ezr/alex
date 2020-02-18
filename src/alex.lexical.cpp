#ifndef __alex_lexical_cpp__
#define __alex_lexical_cpp__

#include "alex.internal.hpp"

namespace alex {

lexical_rules lexical_rules::compile( std::istream& is ) {
    int state = 1;
    bool stay = false;
    lexical_rules rules;
    std::map<int,std::string> names;
    std::set<std::string> pool;
    std::map<int,std::set<std::tuple<int,std::string>>> prefix;
    int id = 0;
    int line = 1;
    int column = 1;
    
    try {
        while( state > 0 ) {

            const auto pre = is.peek();

            switch( state ) {
                case 1: switch( pre ) {
                    case ' ': case '\t': case '\n': case '\r': break;
                    case EOF: state = 0; break;
                    case '/': state = 2; break;
                    case '0' ... '9': id = pre-'0'; state = 5; break;
                    default: throw std::runtime_error(std::string("unexpected character :") + (char)pre);
                } break;
                case 2: switch( pre ) {
                    case '*': state = 3; break;
                    default: throw std::runtime_error(std::string("unexpected character after '/' :") + (char)pre);
                } break;
                case 3: switch( pre ) {
                    case '*': state = 4; break;
                    default: break;
                } break;
                case 4: switch( pre ) {
                    case '/': state = 1; break;
                    default: state = 3; break;
                } break;
                case 5: switch( pre ) {
                    case '0' ... '9': id = id*10 + pre - '0'; break;
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                    case '_': throw std::runtime_error("whitespace expected between name and id");
                    default:
                        if( names.count(id) ) throw std::runtime_error("token definition already exists for " + std::to_string(id));
                        names[id] = "";
                        stay = true;
                        state = 6;
                        break;
                } break;
                case 6: switch( pre ) {
                    case ' ': case '\t': case '\n': case '\r': break;
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                    case '_':
                        names[id] += pre;
                        state = 7;
                        break;
                    default: throw std::runtime_error(std::string("token name expected, unknown character got :") + (char)pre);
                } break;
                case 7: switch( pre ) {
                    case 'a' ... 'z':
                    case 'A' ... 'Z':
                    case '0' ... '9':
                    case '_': names[id] += pre; break;
                    default:
                        if( pool.count(names[id]) ) throw std::runtime_error("token name '" + names[id] + "' occupied" );
                        pool.insert(names[id]);
                        rules[id].name = names[id];
                        stay = true;
                        state = 8;
                        break;
                } break;
                case 8: switch( pre ) {
                    case ' ': case '\t': case '\n': case '\r': break;
                    case '=': state = 9; break;
                } break;
                case 9: switch( pre ) {
                    case ' ': case '\t': case '\n': case '\r': break;
                    case '/': state = 10; break;
                } break;
                case 10: {
                    rules[id].match = regex::compile(is);
                    state = 11;
                } break;
                case 11: {
                    rules[id].suffix = regex::compile(is);
                    state = 1;
                } break;
            }

            if( stay ) stay = false;
            else {
                if( pre == '\n' ) line += (column = 1);
                else column += 1;
                is.get();
            }
        }
    }catch( std::exception& e ) {
        throw std::runtime_error( std::to_string(line) + ":" + std::to_string(column) + ":" + e.what());
    }

    return rules;
}

fsm lexical_rules::compile() {
    fsm diagram;

    for( auto [id, rule] : *this ) {
        auto ret = rule.match.attach( diagram, 1, id );
        if( ret.empty() ) throw std::runtime_error("impossible match for " + std::to_string(id));
        for( auto start : ret )
            rule.suffix.attach( diagram, start, -id );
    }

    return diagram;
}

}

#endif