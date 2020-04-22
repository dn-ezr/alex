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
                        if( id <= 0 ) throw std::runtime_error("token id cannot be less than 1");
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

    for( auto [id, rule] : *this ) {
        auto [re,rs] = rule.match.attach( diagram, 1, id );
        if( re == 0 and rs.empty() ) throw std::runtime_error("impossible match for " + std::to_string(id));
        rule.suffix.attach( diagram, re, -id );
        for( auto r : rs )
            rule.suffix.attach( diagram, r, -id );
    }

    diagram.optimize();
    diagram[1][-1] = {{tree::reach(root,(char*)"end"),{}}};

    return diagram;
}

std::map<std::string, std::string> lex::gencpp( const std::string& lang ) {
    std::map<std::string, std::string> files;
    files["vt.hpp"] = genvtd(lang);
    files["token.hpp"] = gentokend(lang);
    files["lexical.hpp"] = genctxd(lang);
    files["lexical.cpp"] = genctxi(lang);
    return files;
}

std::string lex::genvtd( const std::string& lang ) {
    ns global;
    std::function<std::string(const std::string&,const ns&,int)> print = [&]( const std::string& name, const ns& sc, int indent ) {
        std::string tab = "    ";
        std::string tabs = tab * indent;
        std::string code = tabs + "namespace " + name + " {\n";
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
    global.def[0] = "$";
    return "#ifndef __vt__\n#define __vt__\n\n" + print(lang+"::VT",global,0) + "\n#endif";
}

std::string lex::gentokend( const std::string& lang ) {
    std::string ret;
    size_t i = 0;
    while( true ) {
        auto cut = code_token_hpp.find("%l", i );
        if( cut != std::string::npos ) {
            ret += code_token_hpp.substr( i, cut-i ) + lang;
            i += cut + 2;
        } else {
            ret += code_token_hpp.substr( i );
            break;
        }
    }
    return ret;
}

std::string lex::genctxd( const std::string& lang ) {
    return replace(code_lexical_hpp, {{"%l", lang}});
}

std::string print( fsm_program& prog ) {
    static auto _goto = tree::reach( root, (char*)"goto");
    static auto _into = tree::reach( root, (char*)"into");
    static auto _end = tree::reach( root, (char*)"end");
    static auto _accept_p = tree::reach( root, (char*)"accept+");
    static auto _accept_m = tree::reach( root, (char*)"accept-");
    static auto inst1 = "{ %cmd(%arg0); break; }";
    static auto inst2 = "{ %cmd(%arg0, %arg1); break; }";
    static auto inst3 = "{ %cmd(%arg0, %arg1, %arg2); break; }";
    if( prog.size() != 1 ) throw std::runtime_error("lex::genctxi::print : I can handle program with one instruction only" + fsm::print(prog));
    auto& [cmd,args] = prog[0];
    if( cmd == _end ) {
        return "{ m_state = 0; break; }";
    } else if( cmd == _goto ) {
        return replace(inst1, {
            {"%cmd", "_goto"},
            {"%arg0", std::to_string((long)args[0])}
        });
    } else if( cmd == _into ) {
        return replace(inst1, {
            {"%cmd", "_into"},
            {"%arg0", std::to_string((long)args[0])}
        });
    } else if( cmd == _accept_m ) {
        return replace(inst3, {
            {"%cmd", "accept"},
            {"%arg0", std::to_string((long)args[0])},
            {"%arg1", std::to_string((long)args[1])},
            {"%arg2", "false"}
        });
    } else if( cmd == _accept_p ) {
        return replace(inst3, {
            {"%cmd", "accept"},
            {"%arg0", std::to_string((long)args[0])},
            {"%arg1", std::to_string((long)args[1])},
            {"%arg2", "true"}
        });
    } else {
        throw std::runtime_error("lex::genctxi::print : cannot handle instruction other than end/goto/into/accept+/accept-");
    }
}

std::string print( int input ) {
    if( isprint(input) ) switch( input ) {
        case '\'': return "\'\\\'\'";
        case '\"': return "\'\\\"\'";
        case '\\': return "\'\\\\\'";
        default: return std::string("'")+(char)input+std::string("'");
    } else switch( input ) {
        case ' ': return "\' \'";
        case '\n': return "\'\\n\'";
        case '\r': return "\'\\r\'";
        case '\t': return "\'\\t\'";
        default: return std::to_string(input);
    }
}

std::string print(fsm_state rules ) {
    std::string code;
    std::vector<int> tail;
    static auto s16 = "\n" + std::string(" ")*16;
    std::map<std::string,std::vector<int>> reverse;

    if( rules.count(-4) ) code += s16 + print(rules[-4]);
    for( auto& [input, prog] : rules )
        if( input != -4 and input != -2 )
            if( input >= -1 ) reverse[print(prog)].push_back(input);
            else tail.push_back(input);

    for( auto& [prog,inputs] : reverse ) {
        std::string cond = "if( %cond)";
        while( inputs.size() ) {
            auto input = range(inputs);
            if( input.size() == 1 ) {
                cond = replace(cond, {{"%cond", print(input[0]) + " == m_pre or %cond"}} );
            } else {
                cond = replace(cond, {{"%cond", "( " + print(input.front()) + " <= m_pre and m_pre <= " + print(input.back()) + " ) or %cond"}});
            }
        }
        cond = replace(cond, {{"or %cond", ""}});
        code += s16 + cond + prog;
    }
    if( rules.count(-9) ) code += s16 + "if( m_pre == ' ' or m_pre == '\\t' or m_pre == '\\n' or m_pre == '\\r' ) " + print(rules[-9]);
    if( rules.count(-8) ) code += s16 + "if( '0' <= m_pre and m_pre <= '9' ) " + print(rules[-8]);
    if( rules.count(-7) ) code += s16 + "if( 'A' <= m_pre and m_pre <= 'Z' ) " + print(rules[-7]);
    if( rules.count(-6) ) code += s16 + "if( 'a' <= m_pre and m_pre <= 'z' ) " + print(rules[-6]);
    if( rules.count(-5) ) code += s16 + "if( 1 <= m_pre and m_pre <= 127 ) " + print(rules[-5]);
    if( rules.count(-2) ) code += s16 + "if( 0 < m_pre ) " + print(rules[-2]);
    code += s16 + "accept(1, 0, false);";
    return code;
}

std::string lex::genctxi( const std::string& lang ) {
    std::string code;
    auto dfa = compile();
    static auto s12 = "\n" + std::string(" ")*12;
    std::map<std::string,std::vector<int>> reverse;
    for( auto& [state,inputs] : dfa ) reverse[print(inputs)].push_back(state);
    for( auto& [rules,states] : reverse ) {
        code += s12;
        for( auto state : states ) code += "case " + std::to_string(state) + ": ";
        code += rules;
        code += s12 + "break;";
    }
    return replace(code_lexical_cpp, {
        {"%l", lang},
        {"%dfa", code}
    });
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