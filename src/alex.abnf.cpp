#ifndef __alex_abnf_cpp__
#define __alex_abnf_cpp__

#include "alex.internal.hpp"
#include "abnf.lexical.hpp"
#include "abnf.vt.hpp"
#include <sstream>

namespace alex {

abnf abnf::compile( chainz<token>::iterator& it ) {
    return compile(it,false);
}

abnf abnf::compile( chainz<token>::iterator& it, bool inner ) {
    using namespace ABNF;
    abnf pattern;
    int state = 1;
    int ender = 0;
    auto serr = []( const token& T ){
        throw std::runtime_error("alex::syntax::compile() : syntax error at " +
        std::to_string(T.bl) + ":" + std::to_string(T.bc) + 
        " \033[1;31m" + json(T.tx).toJsonString() + "\033[0m");
    };
    while( it != it.r.end() and state > 0 ) {
        bool stay = false;
        switch( state ) {
            case 1:
                switch( it->id ) {
                    case VT::SPACE: break;
                    case VT::STRING: {
                        std::stringstream ss(it->tx);
                        auto raw = (std::string)json::FromJsonStream(ss);
                        if( pattern.type == abnf::type_t::sequence ) {
                            pattern.sub.push({
                                .type = abnf::type_t::string,
                                .value = raw
                            });
                        } else {
                            pattern.type = abnf::type_t::string;
                            pattern.value = raw;
                        }
                        state = 4;
                    } break;
                    case VT::LABEL: {
                        if( pattern.type == abnf::type_t::sequence ) {
                            pattern.sub.push({
                                .type = abnf::type_t::token,
                                .value = it->tx
                            });
                        } else {
                            pattern.type = abnf::type_t::token;
                            pattern.value = it->tx;
                        }
                        state = 2;
                    } break;
                    case VT::O::SEQ: ender = VT::C::SEQ; state = 3; break;
                    case VT::O::ANY: ender = VT::C::ANY; state = 3; break;
                    case VT::O::OPT: ender = VT::C::OPT; state = 3; break;
                    default: serr(*it); break;
                } break;
            case 2:
                switch( it->id ) {
                    case VT::SPACE: break;
                    case VT::LABEL: 
                        if( pattern.type == abnf::type_t::sequence )
                            pattern.sub[-1].value += " " + it->tx;
                        else
                            pattern.value += " " + it->tx; 
                        break;
                    default: stay = true; state = 4; break;
                } break;
            case 3: {
                auto content = compile(it,false);
                if( it->id != ender ) serr(*it);
                abnf::type_t type;
                switch( ender ) {
                    case VT::C::SEQ: type = abnf::type_t::sequence; break;
                    case VT::C::ANY: type = abnf::type_t::any; break;
                    case VT::C::OPT: type = abnf::type_t::optional; break;
                    default: serr(*it);
                }
                if( pattern.type == abnf::type_t::sequence ) {
                    abnf p = {.type = type};
                    p.sub.push(content);
                    pattern.sub.push(p);
                } else {
                    pattern.type = type;
                    pattern.sub.push(content);
                }
                state = 4;
            } break;
            case 4:
                switch( it->id ) {
                    case VT::SPACE: break;
                    case VT::C::ANY: case VT::C::OPT: case VT::C::SEQ: case VT::EM: case VT::QM: case VT::SEMI: state = 0; break;
                    case VT::O::ANY:case VT::O::OPT:case VT::O::SEQ:case VT::STRING:case VT::LABEL:
                        stay = true; 
                        [[fallthrough]];
                    case VT::COMMA: 
                        if( pattern.type == abnf::type_t::sequence ) {
                            state = 1;
                            break;
                        } else if( pattern.type == abnf::type_t::options ) {
                            serr(*it);
                        } else {
                            auto temp = pattern;
                            pattern.type = abnf::type_t::sequence;
                            pattern.sub.clear();
                            pattern.value.clear();
                            pattern.sub.push(temp);
                            state = 1;
                            break;
                        } break;
                    case VT::OR: 
                        if( inner ) {
                            state = 0;
                        } else if( pattern.type == abnf::type_t::options ) {
                            state = 5;
                        } else {
                            auto temp = pattern;
                            pattern.type = abnf::type_t::options;
                            pattern.sub.clear();
                            pattern.value.clear();
                            pattern.sub.push(temp);
                            state = 5;
                        } break;
                    default: serr(*it); break;
                } break;
            case 5:
                pattern.sub.push(compile(it,true));
                stay = true;
                state = 4;
                break;
            default: serr(*it); break;
        }
        if( state <= 0 ) break;
        if( !stay ) ++it;
    }

    if( state != 0 )
        throw std::runtime_error("alex::abnf::compile() : syntax error unexpected ending");
    while( pattern.type == abnf::type_t::sequence and pattern.sub.size() == 1 ) {
        auto temp = pattern.sub[0];
        pattern = temp;
    }
    return pattern;
}

syntax syntax::compile( std::istream& is ) {
    using namespace ABNF;
    auto lex = LexicalContext();
    chainz<token> tokens;
    bool good = true;
    lex.ontoken = [&](token&& T) {
        if( T.id <= 0) good = false;
        if( T.id != VT::COMMENT )
            tokens.push(std::move(T)); 
        return good;
    };
    lex.perform(is);
    if( !good ) 
        throw std::runtime_error("alex::syntax::compile() : lexical error " + 
            std::to_string(tokens[-1].bl) + ":" + std::to_string(tokens[-1].bc) +
            " \033[1;31m" + json(tokens[-1].tx).toJsonString() + "\033[0m");
    auto it = tokens.begin();
    return compile(it);
}

syntax syntax::compile( chainz<token>::iterator& it ) {
    using namespace ABNF;
    syntax S;
    std::string name;
    int state = 1;
    auto serr = []( const token& T ){
        throw std::runtime_error("alex::syntax::compile() : syntax error at " +
        std::to_string(T.bl) + ":" + std::to_string(T.bc) + 
        " \033[1;31m" + json(T.tx).toJsonString() + "\033[0m");
    };
    while( it != it.r.end() and state > 0 ) {
        bool stay = false;
        switch( state ) {
            case 1:
                switch( it->id ) {
                    case VT::SPACE: break;
                    case VT::LABEL: name = it->tx; state = 2; break;
                    default: serr(*it); break;
                } break;
            case 2:
                switch( it->id ) {
                    case VT::SPACE: break;
                    case VT::LABEL: name += " " + it->tx; break;
                    case VT::ASSIGN: state = 3; break;
                    default: serr(*it); break;
                } break;
            case 3:
                if( S.count(name) )
                    throw std::runtime_error("alex::syntax::compile() : syntax \"" + name + "\" already exists" );
                S[name].pattern = abnf::compile(it);
                switch( it->id ) {
                    case VT::SEMI: state = 1; break;
                    case VT::EM: state = 4; break;
                    case VT::QM: state = 5; break;
                    default: serr(*it); break;
                } break;
            case 4:
                S[name].refuse = abnf::compile(it);
                switch( it->id ) {
                    case VT::SEMI: state = 1; break;
                    default: serr(*it); break;
                } break;
            case 5:
                S[name].suffix = abnf::compile(it);
                switch( it->id ) {
                    case VT::SEMI: state = 1; break;
                    default: serr(*it); break;
                }
        }
        if( state <= 0 ) break;
        if( !stay ) ++it;
    }
    if( state != 1 )
        throw std::runtime_error("alex::syntax::compile() : syntax error unexpected ending");
    return S;
}

std::ostream& operator<<( std::ostream& os, const syntax& s ) {
    for( auto [name,i] : s )
        os << name << " = " << i << std::endl;
    return os;
}

std::ostream& operator<<( std::ostream& os, const synti& s ) {
    os << s.pattern;
    if( s.refuse.type != abnf::type_t::empty ) os << " ! " << s.refuse;
    else if( s.suffix.type != abnf::type_t::empty ) os << " ? " << s.suffix;
    os << ";";
    return os;
}

std::ostream& operator<<( std::ostream& os, const abnf& p ) {
    switch( p.type ) {
        case abnf::type_t::sequence:
            for( int i = 0; i < p.sub.size(); i++ ) {
                if( i != 0)
                    if( p.sub[i].type == abnf::type_t::token ) os << ", ";
                    else os << " ";
                if( p.sub[i].type == abnf::type_t::options ) os << "(";
                os << p.sub[i];
                if( p.sub[i].type == abnf::type_t::options ) os << ")";
            }
        break;
        case abnf::type_t::string:
            os << json(p.value).toJsonString();
        break;
        case abnf::type_t::token:
            os << p.value;
        break;
        case abnf::type_t::options:
            for( int i = 0; i < p.sub.size(); i++ ) {
                if( i != 0 ) os << " | ";
                os << p.sub[i];
            }
        break;
        case abnf::type_t::optional:
            os << "[ ";
            for( auto s : p.sub ) os << s;
            os << " ]"; break;
        case abnf::type_t::any:
            os << "{ ";
            for( auto s : p.sub ) os << s;
            os << " }"; break;
        case abnf::type_t::empty: break;
    }
    return os;
}

}

#endif