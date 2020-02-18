#ifndef __alex_regex_cpp__
#define __alex_regex_cpp__

#include "alex.internal.hpp"

namespace alex {

regex regex::extract( std::istream& is, bool inner ) {
    int state = 1;
    bool stay = false;
    int escape_target;
    regex reg;

    while( state > 0 ) {

        const auto pre = is.peek();

        switch( state ) {
            case 1: switch( pre ) {
                case EOF: throw std::runtime_error("unexpected EOF");
                case '/': reg.type = type_t::none; stay = true; state = 0; break;
                case ']': case ')': throw std::runtime_error("unexpected terminator");
                case '(':
                    if( inner ) {
                        reg.type = type_t::character;
                        reg.value = pre;
                        state = 8;
                    } else {
                        state = 2;
                    } break;
                case '\\': 
                    state = 3;
                    reg.type = type_t::character;
                    break;
                case '[': 
                    if( inner ) {
                        reg.type = type_t::character;
                        reg.value = pre;
                        state = 8;
                    } else {
                        state = 6;
                    } break;
                case '.':
                    reg.type = type_t::family;
                    reg.value = (int)family_t::any;
                    state = 8;
                    break;
                default:
                    reg.type = type_t::character;
                    reg.value = pre;
                    state = 8;
                    break;
            } break;
            case 2: {
                reg = compile(is, ')' );
                switch( is.peek() ) {
                    case ')': state = 8; break;
                    default: throw std::runtime_error("terminator expected");
                }
            } break;
            case 3: switch( pre ) {
                case 'x': state = 4; break;
                case 'n': case 'r': case 't': 
                case '\\': case '-':
                case '?': case '+': case '|': case '.':case '*':
                case '(':case ')':case '[':case ']': {
                    switch( pre ) {
                        case 'n': reg.value = 'n'; break;
                        case 'r': reg.value = 'r'; break;
                        case 't': reg.value = 't'; break;
                        default: reg.value = pre; break;
                    }
                    state = 8;
                } break;
                case 's': case 'd': case 'p': case 'l': case 'u': case 'b': {
                    switch( pre ) {
                        case 's': reg.value = (int)family_t::space; break;
                        case 'd': reg.value = (int)family_t::digit; break;
                        case 'p': reg.value = (int)family_t::punct; break;
                        case 'l': reg.value = (int)family_t::lower; break;
                        case 'u': reg.value = (int)family_t::upper; break;
                        case 'b': 
                            if( inner ) throw std::runtime_error("boundary not supported inner collection");
                            else reg.value = (int)family_t::boundary;
                            break;
                    }
                    reg.type = type_t::family;
                    state = 8;
                } break;
                default: throw std::runtime_error("unexpected escape character");
            } break;
            case 4: switch( pre ) {
                case '0' ... '9': reg.value = (pre - '0') << 4; break;
                case 'a' ... 'f': reg.value = (pre - 'a' + 10) << 4; break;
                case 'A' ... 'F': reg.value = (pre - 'A' + 10) << 4; break;
                default: throw std::runtime_error("unexpected escape hex");
            } state = 5; break;
            case 5: switch( pre ) {
                case '0' ... '9': reg.value += pre - '0'; break;
                case 'a' ... 'f': reg.value += pre - 'a' + 10; break;
                case 'A' ... 'F': reg.value += pre - 'A' + 10; break;
                default: throw std::runtime_error("unexpected escape hex");
            } state = 8; break;
            case 6: switch( pre ) {
                case '^': reg.type = type_t::reverse; state = 7; break;
                default: reg.type = type_t::collection; stay = true; state = 7; break;
            } break;
            case 7: switch( pre ) {
                case ']': state = 8; break;
                default: reg.content << extract(is, true); stay = true; break;
            } break;
            case 8: if( inner ) {
                if( pre == '-' ) {
                    if( reg.type != type_t::character ) throw std::runtime_error("unexpected dash inner collection");
                    reg.content << reg;
                    reg.type = type_t::range;
                    state = 9;
                } else {
                    stay = true;
                    state = 0;
                }
            } else switch( pre ) {
                case '?': reg.content << reg; reg.type = type_t::optional; state = 0; break;
                case '*': reg.content << reg; reg.type = type_t::any; state = 0; break;
                case '+': reg.content << reg; reg.type = type_t::more; state = 0; break;
                case '|': reg.content << reg; reg.type = type_t::options; state = 10; break;
                default: state = 0; stay = true; break;
            } break;
            case 9: {
                reg.content << extract(is, true);
                if( reg.content[1].type != type_t::character ) throw std::runtime_error("unexpected expression for range");
                stay = true;
                state = 0;
            } break;
            case 10: {
                auto right = extract(is);
                if( right.type == type_t::options ) reg.content += right.content;
                else reg.content << right;
                stay = true;
                state = 0;
            } break;
        }

        if( stay ) stay = false;
        else is.get();
    }

    return reg;
}

regex regex::compile( std::istream& is, char terminator ) {

    regex reg;
    reg.type = type_t::sequence;

    while( is.peek() != terminator ) {
        auto item = extract(is);
        reg.content << item;
    }

    return reg;
}

std::set<int> regex::attach( fsm& machine, int start ) {
    command_desc::init();
    auto into = tree::reach(root, (char*)"into");
    long end = 0;
    switch( type ) {
        case type_t::character: {
            if( machine.count(start) and machine[start].count(value) )
                for( auto [cmd, args] : machine[start][value] )
                    if( cmd == into ) return {(int)(long)args[0]};
            end = machine.genstate();
            machine[start][value] = {{into, {(long)end}}};
        } break;
        case type_t::family:
        case type_t::optional:
        case type_t::any:
        case type_t::more:
        case type_t::sequence: {
            std::set<int> starts = {start};
            for( auto expr : content ) {
                std::set<int> step;
                for( auto start : starts ) {
                    step.merge(expr.attach( machine, start ));
                }
                starts = step;
            }
            return starts;
        }
        case type_t::reverse:
        case type_t::collection:
        case type_t::options: break;
    }

    return {0};
}

std::set<int> regex::attach( fsm& machine, int start, int accept ) {
    command_desc::init();
    auto cmd_accept = tree::reach(root, (char*)"accept-");
    auto reaches = attach( machine, start );
    
    for( auto reach : reaches )
        machine[reach][-2] = {{cmd_accept, {(long)accept, 1L}}};

    return reaches;
}

void regex::print( std::ostream& os ) {
    switch( type ) {
        case type_t::character:
            switch( value ) {
                case '\n': os << "\\n"; break;
                case '\r': os << "\\r"; break;
                case '\t': os << "\\t"; break;
                case '\\':case '-':
                case '?':case '+':case '|':case '.':case '*':
                case '(':case ')':case '[':case ']': os << '\\' << (char)value; break;
                default: os << (char)value; break;
            } break;
        case type_t::family:
            switch( value ) {
                case (int)family_t::any: os << "."; break;
                case (int)family_t::space: os << "\\s"; break;
                case (int)family_t::digit: os << "\\d"; break;
                case (int)family_t::punct: os << "\\p"; break;
                case (int)family_t::lower: os << "\\l"; break;
                case (int)family_t::upper: os << "\\u"; break;
                case (int)family_t::boundary: os << "\\b"; break;
            } break;
        case type_t::optional: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "?";
        } break;
        case type_t::any: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "*";
        } break;
        case type_t::more: {
            auto surround =  content[0].content.size() > 1;
            if( surround ) os << "(";
            content[0].print(os);
            if( surround ) os << ")";
            os << "+";
        } break;
        case type_t::sequence: {
            for( auto expr : content ) expr.print(os);
        } break;
        case type_t::reverse: {
            os << "[^";
            for( auto expr : content ) expr.print(os);
            os << "]";
        } break;
        case type_t::collection: {
            os << "[";
            for( auto expr : content ) expr.print(os);
            os << "]";
        } break;
        case type_t::range: {
            content[0].print(os);
            os << '-';
            content[1].print(os);
        } break;
        case type_t::options:
            for( int i = 0; i < content.size(); i++ ) {
                if( i != 0 ) os << "|";
                content[i].print(os);
            } break;
    }
}

}

#endif