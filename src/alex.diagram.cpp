#ifndef __alex_cpp__
#define __alex_cpp__

#include "alex.internal.hpp"
#include <set>
#include <functional>

namespace alex {

tree* root = nullptr;

fsm fsm::compile( std::istream& is ) {
    fsm diagram;
    command_desc::init();

    int line = 1;
    int column = 1;
    int state = 1;
    bool stay = false;
    bool escape;
    int target;
    std::string str;
    char ascii;
    const tree* node = nullptr;
    std::string args;
    chainz<json> params;

    int pf;
    int pi;
    std::string ps;

    struct {
        int state;
        int input;
        int range;
        int code;
        std::set<int> states;
        std::set<int> inputs;
        bool multi_input;
        bool multi_cmd;
    } var;

    while ( state > 0 ) {
        const int pre = is.peek();
        switch( state ) {
            case 1: switch( pre ) {
                case ' ':case '\t':case '\r': case '\n': break;
                case '1' ... '9': var.state = pre - '0'; state = 2; break;
                case '\0': case EOF: state = 0; break;
                default: state = -1; break;
            } break;
            case 2: switch( pre ) {
                case '1' ... '9': var.state = var.state*10 + pre - '0'; break;
                default: stay = true; state = 3; break;
            } break;
            case 3: switch( pre ) {
                case ' ':case '\t':case '\r': case '\n': break;
                case ',': var.states.insert(var.state); state = 1; break;
                case '(': var.multi_input = true; var.states.insert(var.state); state = 4; break;
                default: var.multi_input = false; var.states.insert(var.state); state = 4; stay = true; break;
            } break;
            case 4: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case 'l': var.input = 'a'; var.range = 'z'; state = 11; break;
                case 'u': var.input = 'A'; var.range = 'Z'; state = 11; break;
                case 'd': var.input = '0'; var.range = '9'; state = 11; break;
                case 's': var.inputs.insert({' ','\n','\t','\r'}); var.input = ' '; state = 7; break;
                case 'e': var.input = -1; state = 7; break;
                case '*': var.input = -2; state = 7; break;
                case '-': var.input = -3; state = 7; break;
                case '>': var.input = -4; state = 7; break;
                case '.': var.input = -5; state = 7; break;
                case '0' ... '9': var.input = pre - '0'; state = 6; break;
                case '\'': state = 1024; target = 5; stay = true; break;
                case ')': if( var.multi_input ) var.states.clear(), state = 1; else state = -4; break;
                default: state = -4; break;
            } break;
            case 5: var.input = ascii; state = 7; stay = true; break;
            case 6: switch( pre ) {
                case '0' ... '9': var.input = var.input + pre - '0'; break;
                default: stay = true; state = 7; break;
            } break;
            case 7: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '~': state = 8; break;
                case ',': var.inputs.insert(var.input); state = 4; break;
                case '(': var.multi_cmd = true; var.inputs.insert(var.input); state = 12; break;
                default: var.multi_cmd = false; var.inputs.insert(var.input); state = 12; stay = true; break;
            } break;
            case 8: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '0' ... '9': var.range = pre - '0'; state = 10; break;
                case '\'': state = 1024; target = 9; stay = true; break;
            } break;
            case 9: var.range = ascii; state = 11; stay = true; break;
            case 10: switch( pre ) {
                case '0' ... '9': var.range = var.range + pre - '0'; break;
                default: stay = true; state = 11; break;
            } break;
            case 11: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case ',': for( int i = var.input; i <= var.range; i++ ) var.inputs.insert(i); state = 4; break;
                case '(': var.multi_cmd = true; for( int i = var.input; i <= var.range; i++ ) var.inputs.insert(i); state = 12; break;
                default: var.multi_cmd = false; for( int i = var.input; i <= var.range; i++ ) var.inputs.insert(i); state = 12; stay = true; break;
            } break;
            case 12: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                default: node = root; state = 13; stay = true; break;
                case ')':
                    if( var.multi_cmd )
                        if( var.multi_input ) (var.inputs.clear()), (state = 4);
                        else (var.states.clear()), (state = 1);
                    else if( var.multi_input ) (var.states.clear()), (state = 1);
                    else state = -12;
                    break;
            } break;
            case 13:
                if( node->sub.count(pre) ) node = &node->sub.at(pre);
                else if( node->tag ) (var.code = node->tag), (state = 14), (args = commands.at(node->tag).args), (stay = true), (params.clear());
                else state = -13;
                break;
            case 14:
                if( params.size() >= args.size() ) {
                    for( auto state : var.states ) 
                        for( auto input : var.inputs ) 
                            diagram[state][input] << fsm_instruction(var.code, params);
                    params.clear();
                    // diagram.store(std::cout);
                    if( var.multi_cmd ) (state = 12), (stay = true);
                    else if( var.multi_input ) (var.inputs.clear()), (state = 4), (stay = true);
                    else (var.inputs.clear()), (var.states.clear()), (state = 1),(stay = true);
                } 
                else if( auto ind = args[params.size()]; ind == 'i' ) (stay = true), (state = 15);
                else if( ind == 's' ) (stay = true), (state = 18);
                else if( ind == 'a' ) (stay = true), (state = 21);
                else if( ind == 'l' ) (stay = true), (state = 22);
                else if( ind == 'n' ) (stay = true), (state = 24);
                else if( ind == 'p' ) (stay = true), (state = 25);
                else state = -14;
                break;
            case 15: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '-': pf = -1; pi = 0; state = 16; break;
                case '0' ... '9': pf = 1; pi = pre - '0'; state = 17; break;
                case '\'': pf = 1; target = 16; stay = true; state = 1024; break;
                default: state = -15; break;
            } break;
            case 16: pi = ascii; state = 17; stay = true; break;
            case 17: switch( pre ) {
                case '0' ... '9': pi = pi*10 + pre - '0'; break;
                default: params << (long)(pi*pf); state = 14; stay = true; break;
            } break;
            case 18: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '"': ps.clear(); state = 19; break;
                default: state = -18; break;
            } break;
            case 19: switch( pre ) {
                case '\"': params << ps; state = 14; break;
                case '\\': state = 20; break;
                default: ps += pre; break;
            } break;
            case 20: switch( pre ) {
                case '\"': ps += '\"'; state = 19; break;
                case '\'': ps += '\''; state = 19; break;
                case 'a' : ps += '\a'; state = 19; break;
                case 'b' : ps += '\b'; state = 19; break;
                case 'f' : ps += '\f'; state = 19; break;
                case 'n' : ps += '\n'; state = 19; break;
                case 'r' : ps += '\r'; state = 19; break;
                case 't' : ps += '\t'; state = 19; break;
                case 'v' : ps += '\v'; state = 19; break;
                case '\\': ps += '\\'; state = 19; break;
                default: state = -20; break;
            } break;
            case 21: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '-': case '0' ... '9': case '\'': stay = true; state = 15; break;
                case '\"': stay = true; state = 18; break;
                default: state = -21; break;
            } break;
            case 22: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case '0' ... '9': stay = true; state = 15; break;
                case 'a' ... 'z': case 'A' ... 'Z': case '_': ps.clear(); stay = true; state = 23; break;
                default: state = -22; break;
            } break;
            case 23: switch( pre ) {
                case 'a' ... 'z': case 'A' ... 'Z': case '_': case '0' ... '9': case '.': ps += pre; break;
                default: params << ps; stay = true; state = 14; break;
            } break;
            case 24: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                case 'a' ... 'z': case 'A' ... 'Z': case '_': ps.clear(); stay = true; state = 23; break;
                default: state = -24; break;
            } break;
            case 25: switch( pre ) {
                case ' ': case '\t': case '\r': case '\n': break;
                default: ps.clear(); stay = true; state = 26; break;
            } break;
            case 26: {
                if( ispunct(pre) ) {ps += pre; break;}
                else {params << ps; stay = true; state = 14; break;}
            } break;

            case 1024: switch( pre ) {
                case '\'': escape = false;state = 1025;break;
                default: state = -1024;break;
            } break;
            case 1025: switch( pre ) {
                default: ascii = pre;state = 1027;break;
                case '\'': state = -1025;break;
                case '\\': state = 1026;break;
                case EOF: case '\0': state = -1025;break;
            } break;
            case 1026: switch( pre ) {
                case '\"': ascii = '\"'; state = 1027; break;
                case '\'': ascii = '\''; state = 1027; break;
                case 'a' : ascii = '\a'; state = 1027; break;
                case 'b' : ascii = '\b'; state = 1027; break;
                case 'f' : ascii = '\f'; state = 1027; break;
                case 'n' : ascii = '\n'; state = 1027; break;
                case 'r' : ascii = '\r'; state = 1027; break;
                case 't' : ascii = '\t'; state = 1027; break;
                case 'v' : ascii = '\v'; state = 1027; break;
                case '\\': ascii = '\\'; state = 1027; break;
                default:state = -1026;break;
            } break;
            case 1027: switch( pre ) {
                case '\'':state = target;break;
                default:state = -1027;break;
            } break;
        }

        if( stay ) {
            stay = false;
        } else if( state > 0 ) {
            if( pre == '\n' ) line += column = 1;
            else column += 1;
            is.get();
        }
    }

    if( state < 0 )
        throw std::runtime_error("error(" + std::to_string(state) + ") at " + std::to_string(line) + ":" + std::to_string(column) + " preview: " + (char)is.peek() );

    return diagram;
}

bool fsm::store( std::ostream& os ) {
    for( auto [state, rules] : *this ) {
        os << state;
        auto mi = rules.size() > 1;
        if( mi ) os << " (" << std::endl;
        for( auto [input,prog] : rules ) {
            if( mi ) os << "  ";
            else os << " ";
            if( isprint(input) ) os << "'" << (char)input << "'";
            else os << input;
            auto mc = prog.size() > 1;
            if( mc ) os << " (" << std::endl;
            for( auto [cmd, args] : prog ) {
                if( mc ) os << "    ";
                else os << " ";
                 os << commands.at(cmd).name << " ";
                for( auto arg : args ) os << arg.toJsonString() << " ";
                if( mc ) os << std::endl;
            }
            if( mc ) os << "  )";
            if( mi ) os << std::endl;
        }
        if( mi ) os << ")";
        os << std::endl;
    }
    os << std::endl;
    return true;
}

int fsm::genstate() {
    for( int i = 2; i > 0; i++ )
        if( !this->count(i) ) return i;
    return 0;
}

fsm_instruction* fsm::findexit( int state, int input ) {
    command_desc::init();
    if( !count(state) or !at(state).count(input) ) return nullptr;
    auto end = tree::reach(root, (char*)"end");
    auto& prog = at(state)[input];
    for( auto& inst : prog )
        if( std::get<0>(inst) < end ) return &inst;
    return nullptr;
}

}

#endif