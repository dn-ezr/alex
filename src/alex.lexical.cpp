#ifndef __alex_lexical_cpp__
#define __alex_lexical_cpp__

#include "alex.internal.hpp"

namespace alex {

lex lex::compile( std::istream& is ) {
    int state = 1;
    bool stay = false;
    lex rules;
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
                    case EOF: throw std::runtime_error(std::string("unterminated comment"));
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
                    case '_': case ':': names[id] += pre; break;
                    default:
                        if( names[id].back() == ':' ) throw std::runtime_error("token name cannot be terminated by colon");
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

fsm lex::compile() {
    fsm diagram;

    std::cout << "[Compiling LEX]" << std::endl;
    for( auto [id, rule] : *this ) {
        std::cout << id << " ";
        auto [re,rs] = rule.match.attach( diagram, 1, id );
        if( re == 0 ) throw std::runtime_error("impossible match for " + std::to_string(id));
        rule.suffix.attach( diagram, re, -id );
        for( auto r : rs )
            rule.suffix.attach( diagram, r, -id );
    }
    std::cout << std::endl;

    return diagram;
}

std::map<std::string, std::string> lex::gencpp( const std::string& lang ) {
    std::map<std::string, std::string> files;
    files["vt.hpp"] = genvt(lang);
    return files;
}

std::string lex::genvt( const std::string& lang ) {
    ns global;
    std::function<std::string(const std::string&,const ns&,int)> print = [&]( const std::string& name, const ns& sc, int indent ) {
        std::string tab = "    ";
        std::string tabs = tab * indent;
        std::string code = tabs + "namespace " + name + "{\n";
        for( auto [id,tk] : sc.def ) {
            code += tabs + tab + "constexpr int " + tk + " = " + std::to_string(id) + ";\n";
        }
        for( auto [key,sub] : sc.sub ) {
            code += print(key,sub,indent+1);
        }
        code += tabs + "}\n";
        return code;
    };
    for( auto [id,def] : *this ) {
        auto names = split(def.name, ':');
        ns* sc = &global;
        for( int off = 0; off + 1 < names.size(); off++ )
            sc = &sc->sub[names[off]];
        sc->def[id] = names[names.size()-1];
    }
    return "#ifndef __vt__\n#define __vt__\n\n" + print(lang,global,0) + "\n#endif";
}

std::ostream& operator << ( std::ostream& os, lexi& rule ) {
    os << rule.name << " = /" << rule.match << "/" << rule.suffix << "/";
    return os;
}

std::ostream& operator << ( std::ostream& os, lex& rules ) {
    for( auto [id, rule] : rules )
        std::cout << id << ' ' << rule << std::endl;
    return os;
}

}

#endif