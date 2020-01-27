#ifndef __alex_cmd_cpp__
#define __alex_cmd_cpp__

#include "alex.hpp"

namespace alex {

#define mkcmd( _name, _prop, body ) { __LINE__-10, (command_desc){ name: #_name, args: _prop, proc: []( vstack& stack, const chainz<json>& args ) -> bool{body}}},

const std::map<int,command_desc> commands = { 
    mkcmd( end, "", 
        stack.perform = false;
        return true;
    )
    mkcmd( drop, "",
        stack.skip = true;
        return true;
    )
    mkcmd( stay, "",
        return true;
    )
    mkcmd( into, "i",
        stack.target_state = (long)args[0];
        return true;
    )
    mkcmd( goto, "i",
        stack.target_state = (long)args[0];
        stack.input = false;
        return true;
    )
    mkcmd( accept+, "l",
        stack.skip = true; 
        if( args[0].is(json::integer) )
            stack.results << std::tuple((long)args[0], stack.buf + (char)stack.pre);
        else if( args[0].is(json::string) ) {
            if( !stack.var.count(args[0]) ) throw std::runtime_error("bad label " + (std::string)args[0] + ", variable undefined");
            if( !stack.var[args[0]].is(json::integer) ) throw std::runtime_error("bad variable " + (std::string)args[0] + ", wrong data type");
            stack.results << std::tuple((long)stack.var[args[0]], stack.buf + (char)stack.pre);
        }
        stack.buf.clear();
        return false;
    )
    mkcmd( accept-, "l",
        stack.input = false; 
        if( stack.buf.size() ) {
            if( args[0].is(json::integer) )
                stack.results << std::tuple((long)args[0], stack.buf);
            else if( args[0].is(json::string) ) {
                if( !stack.var.count(args[0]) ) throw std::runtime_error("bad label " + (std::string)args[0] + ", variable undefined");
                if( !stack.var[args[0]].is(json::integer) ) throw std::runtime_error("bad variable " + (std::string)args[0] + ", wrong data type");
                stack.results << std::tuple((long)stack.var[args[0]], stack.buf);
            }
        }
        stack.buf.clear();
        return false;
    )
    mkcmd( mem.init, "i",
        if( stack.mem ) delete[] stack.mem;
        auto len = (long)args[0];
        if( len > 0 ) stack.mem = new char[len];
        else stack.mem = nullptr;
        stack.memlen = len;
        return false;
    )
    mkcmd( store, "la",
        if( args[0].is(json::integer) ) {
            auto addr = (long)args[0];
            if( args[1].is(json::integer) ) {
                if( addr+4 > stack.memlen ) throw std::runtime_error("bad address: " + std::to_string(addr) + ", out of range");
                *(int*)(stack.mem+addr) = (long)args[1];
            } else if( args[1].is(json::string) ) {
                auto str = (std::string)args[1];
                if( addr+str.size()+1 > stack.memlen ) throw std::runtime_error("bad address: " + std::to_string(addr) + ", out of range");
                strcpy(stack.mem+addr, str.c_str() );
            }
        } else if( args[0].is(json::string) ) {
            auto& var = stack.var[args[0]];
            var = args[1];
        }
        return false;
    )
    mkcmd( store.len, "ls",
        if( args[0].is(json::integer) ) {
            auto addr = (long)args[0];
            if( addr + sizeof(size_t) >= stack.memlen ) throw std::runtime_error("bad address: " + std::to_string(addr) + ", out of range");
            *(size_t*)(stack.mem+addr) = ((std::string)args[1]).size();
        } else if( args[0].is(json::string) ) {
            auto& var = stack.var[args[0]];
            var = (long)((std::string)args[1]).size();
        }
        return false;
    )
    mkcmd( load, "nn",
        if( !stack.var.count(args[1]) )
            throw std::runtime_error("undefined variable " + (std::string)args[1]);
        stack.var[args[0]] = stack.var[args[1]];
        return false;
    )
};

#undef mkcmd
}

#endif