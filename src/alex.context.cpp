#ifndef __alex_context_cpp__
#define __alex_context_cpp__

#include "alex.internal.hpp"

namespace alex {

void vstack::autowire( token pre, token buf, chainz<state_frame>& states ) {
    var["buf.begl"] = (long)buf.begl;
    var["buf.begc"] = (long)buf.begc;
    var["buf.endl"] = (long)buf.endl;
    var["buf.endc"] = (long)buf.endc;
    var["buf"] = buf.tx;

    var["pre"] = (long)pre.id;
    var["pre.tx"] = pre.tx;
    var["pre.begl"] = (long)pre.begl;
    var["pre.begc"] = (long)pre.begc;
    var["pre.endl"] = (long)pre.endl;
    var["pre.endc"] = (long)pre.endc;
    var["state"] = (long)states[-1].state;
}

context::context( const alex::fsm& d ):diag(d) {

}

bool context::initiate() {
    var.clear();
    if( mem ) delete[] mem;
    mem = nullptr;
    memlen = 0;
    if( diag.count(1) ) state = &diag.at(1);
    else return false;
    line = column = 1;
    buf.clear();
    return true;
}

tokens context::perform( std::istream& is , std::ostream& os ) {

    auto mktoken = [&]( int input ) {
        return token {
            id: input,
            begl: line,
            begc: column,
            endl: (input == '\n')?line+1:line,
            endc: (input == '\n')?1:column+1,
            tx: ""+input,
        };
    };

    if( !initiate() ) {
        throw std::runtime_error("failed to initiate");
    }
    tokens results;

    std::set<int> trace;

    chainz<state_frame> states = {(state_frame){1,{}}};

    while( states.size() and states[-1].state >= 1 ) {
        token const pre = mktoken(is.peek());
        auto vs = vstack { var: var, mem: mem, memlen: memlen, state: states[-1].state, os: os };
        vs.autowire( pre, buf, states );

        auto run = [&]( int input ) {
            os << "\tinput: " << pre.id << "(" << pre.tx << ")" << std::endl;
            int ip = 0;
            auto program = state->at(input);
            while( ip < program.size() ) {
                auto& [cmd,args] = program[ip];
                if( !commands.count(cmd) ) throw std::runtime_error("command "+std::to_string(cmd)+" not found");
                os << "\t\t" << commands.at(cmd).name << " ";
                for( auto arg : args ) os << arg.toJsonString() << " ";
                os << std::endl;
                auto ret = commands.at(cmd).proc(vs, args);
                if( ret.empty() ) break;
                if( ret == "+" ) ip += 1;
                else {
                    bool found = false;
                    for( int rip = 0; rip < program.size(); rip++ ) {
                        auto& [cmd,args] = program[rip];
                        if( commands.count(cmd) and commands.at(cmd).name == "@" and (std::string)args[0] == ret ) {
                            found = true;
                            ip = rip;
                            break;
                        }
                    }
                    if( !found ) throw std::runtime_error("anchor " + ret + " not found");
                }
            }
        };

        try {
            if( !diag.count(states[-1].state) )
                throw std::runtime_error("state " + std::to_string(states[-1].state) + " not found");
            
            if( state != &diag.at(states[-1].state) ) {
                state = &diag.at(states[-1].state);
                os << "state: " << std::to_string(states[-1].state) << std::endl;
                if( state->count(-4) ) run(-4);
            } else {
                state = &diag.at(states[-1].state);
            }

            if( !trace.count(states[-1].state) ) {
                if( state->count(-3) ) run(-3);
                trace.insert(states[-1].state);
            }
            
            if( state->count(pre.id) ) run(pre.id);
            else if( state->count(-9) and (pre.id == ' ' or pre.id == '\t' or pre.id == '\n' or pre.id == '\r') ) run(-9);
            else if( state->count(-8) and pre.id <= '9' and pre.id >= '0' ) run(-8);
            else if( state->count(-7) and pre.id <= 'Z' and pre.id >= 'A' ) run(-7);
            else if( state->count(-6) and pre.id <= 'z' and pre.id >= 'a' ) run(-6);
            else if( state->count(-5) and pre.id <= 255 and pre.id >= 1 ) run(-5);
            else if( state->count(-2) and pre.id > 0 ) run(-2);
            else throw std::runtime_error("invalid input "+std::to_string(pre.id));
            if( state->count(states[-1].state) and state[states[-1].state].size() == 0 )
                throw std::runtime_error("invalid input "+std::to_string(pre.id));
        } catch( std::exception& e ) {
            throw std::runtime_error("[error (" + std::to_string(line) + ":" + std::to_string(column) + ")]: " + e.what());
        }

        if( !vs.action == vstack::terminate or pre.id == EOF ) break;
        bool input = true;

        switch( vs.action ) {
            case vstack::input: {
                buf.tx += pre.tx;
                buf.endc = pre.endc;
                buf.endl = pre.endl;
                if( buf.begc <= 0 ) {
                    buf.begc = pre.begc;
                    buf.begl = pre.begl;
                }
            } break;
            case vstack::pause: {
                input = false; 
            } break;
            case vstack::drop: {
                /** Nothing to be done */
            } break;
            case vstack::accept: {
                buf.id = vs.n;
                if( vs.c ) {
                    buf.tx += pre.tx;
                    buf.endc = pre.endc;
                    buf.endl = pre.endl;
                    if( buf.begc <= 0 ) {
                        buf.begc = pre.begc;
                        buf.begl = pre.begl;
                    }
                } else {
                    input = false;
                }
                if( buf.tx.size() ) results << buf;
                buf.clear();
                states[-1].state = vs.s;
            } break;
            case vstack::movin:
                if( vs.s >= 0 ) states << state_frame{vs.s,{pre}};
                else states << state_frame{-vs.s,{}};
                break;
            case vstack::movout:
                while ( vs.c-- > 0 )
                    while( states[-1].cache.size() ) 
                        is.putback(states[-1].cache[-1].id), states[-1].cache.pop();
            break;
            case vstack::redu: throw std::runtime_error("reduct command is not supported in flat mode"); break;
            case vstack::stay: {
                input = false;
                while( vs.c-- > 0 ) {
                    auto i = mktoken(is.get());
                    states[-1].cache << i;
                    line = i.endl;
                    column = pre.endc;
                }
             } break;
            default: throw std::runtime_error("impossible action");
        }
        if( input ) (is.get()), (line = pre.endl), (column = pre.endc);
    }

    return results;
}

}

#endif