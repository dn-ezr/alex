#ifndef __alex_cmd_cpp__
#define __alex_cmd_cpp__

#include "alex.internal.hpp"

namespace alex {

#define mkcmd( _name, _prop, body ) { __LINE__, (command_desc){ name: #_name, args: _prop, proc: []( vstack& stack, const chainz<json>& args ) -> std::string{body}}},

#define isdefined( __var ) if( !stack.var.count(__var) ) throw std::runtime_error("bad label " + (std::string)__var + ", variable undefined")
#define isinteger( __var ) if( !stack.var[__var].is(json::integer) ) throw std::runtime_error("bad variable " + (std::string)__var + ", wrong data type")

extern const std::map<int,command_desc> commands = {
    mkcmd( end, "", 
        stack.action = vstack::terminate;
        return "";
    )
    mkcmd( drop, "",
        stack.action = vstack::drop;
        return "";
    )
    mkcmd( input, "",
        stack.action = vstack::input;
        return "";
    )
    mkcmd( into, "i",
        stack.state = (long)args[0];
        stack.action = vstack::input;
        return "";
    )
    mkcmd( goto, "i",
        stack.state = (long)args[0];
        stack.action = vstack::pause;
        return "";
    )
    mkcmd( stay, "i",
        stack.c = (long)args[0];
        stack.action = vstack::stay;
        return "";
    )
    mkcmd( accept+, "ll",
        stack.c = 1;
        if( args[0].is(json::integer) ) 
            stack.n = (long)args[0];
        else if( args[0].is(json::string) ) {
            isdefined( args[0] );
            isinteger( args[0] );
            stack.n = (long)stack.var[args[0]];
        }

        if( args[1].is(json::integer) )
            stack.s = (long)args[1];
        else if( args[1].is(json::string) ) {
            isdefined( args[1] );
            isinteger( args[1] );
            stack.s = (long)stack.var[args[1]];
        }
        stack.action = vstack::accept;
        return "";
    )
    mkcmd( accept-, "ll",
        stack.c = 0;
        if( args[0].is(json::integer) ) 
            stack.n = (long)args[0];
        else if( args[0].is(json::string) ) {
            isdefined( args[0] );
            isinteger( args[0] );
            stack.n = (long)stack.var[args[0]];
        }

        if( args[1].is(json::integer) )
            stack.s = (long)args[1];
        else if( args[1].is(json::string) ) {
            isdefined( args[1] );
            isinteger( args[1] );
            stack.s = (long)stack.var[args[1]];
        }
        stack.action = vstack::accept;
        return "";
    )
    mkcmd( mem.init, "i",
        if( stack.mem ) delete[] stack.mem;
        auto len = (long)args[0];
        if( len > 0 ) stack.mem = new char[len];
        else stack.mem = nullptr;
        stack.memlen = len;
        return "+";
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
        return "+";
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
        return "+";
    )
    mkcmd( load, "nn",
        if( !stack.var.count(args[1]) )
            throw std::runtime_error("undefined variable " + (std::string)args[1]);
        stack.var[args[0]] = stack.var[args[1]];
        return "+";
    )
    mkcmd( eval, "nnpn",
        isdefined(args[1]);
        isdefined(args[3]);
        std::string op = args[2];
        if( op == "." ) {
            std::string res;
            if( stack.var[args[1]].is(json::string) ) res = (std::string)stack.var[args[1]];
            else if( stack.var[args[1]].is(json::integer) ) res = (long)stack.var[args[1]];
            if( stack.var[args[3]].is(json::string) ) res += (std::string)stack.var[args[3]];
            else if( stack.var[args[3]].is(json::integer) ) res += (long)stack.var[args[3]];
            stack.var[args[0]] = res;
            return "+";
        }
        isinteger(args[1]);
        isinteger(args[3]);
        auto left = (long)stack.var[args[1]];
        auto right = (long)stack.var[args[3]];
        auto& res = stack.var[args[0]];
        switch( op[0] ) {
            case '+': res = left + right; break;
            case '-': res = left - right; break;
            case '*': res = left * right; break;
            case '/': res = left / right; break;
            case '%': res = left % right; break;
            case '&': res = left & right; break;
            case '|': res = left | right; break;
            case '^': res = left ^ right; break;
            default: throw std::runtime_error("unknown operator " + op);
        }
        return "+";
    )
    mkcmd( j.cmp, "nnpn",
        isdefined(args[1]);
        isdefined(args[3]);
        long ia;
        long ib;
        std::string sa;
        std::string sb;
        int sit = 0;
        std::string op = args[2];
        if( stack.var[args[1]].is(json::integer) ) {
            ia = (long)stack.var[args[1]];
        } else if( stack.var[args[1]].is(json::string) ) {
            sit = 0x0001'0000;
            sa = (std::string)stack.var[args[1]];
        }
        if( stack.var[args[2]].is(json::integer) ) {
            ib = (long)stack.var[args[2]];
        } else if( stack.var[args[2]].is(json::string) ) {
            sit |= 0x0010'0000;
            sb = (std::string)stack.var[args[2]];
        }
        if( sit == 0x00100000 or sit == 0x00010000 ) throw std::runtime_error("operand types mismatch");
        if( op == "<" ) sit += 0;
        else if( op == ">" ) sit += 1;
        else if( op == "<=" ) sit += 2;
        else if( op == ">=" ) sit += 3;
        else if( op == "!=" ) sit += 4;
        else if( op == "==" ) sit += 5;
        else throw std::runtime_error("unknown compare operator " + op);
        bool res;
        switch( sit ) {
            case 0: res = (ia < ib); break;
            case 0x110000+0: res = ( sa < sb); break;
            case 1: res = (ia > ib); break;
            case 0x110000+1: res = ( sa > sb); break;
            case 2: res = (ia <= ib); break;
            case 0x110000+2: res = ( sa <= sb); break;
            case 3: res = (ia >= ib); break;
            case 0x110000+3: res = ( sa >= sb); break;
            case 4: res = (ia != ib); break;
            case 0x110000+4: res = ( sa != sb); break;
            case 5: res = (ia == ib); break;
            case 0x110000+5: res = ( sa == sb); break;
        }
        if( res ) return args[0];
        else return "+";
    )
};

#undef mkcmd

void command_desc::init() {
    if( root ) return;
    root = new tree;
    for( auto [code, cmd] : commands ) {
        auto node = root;
        for( auto c : cmd.name ) node = &node->sub[c];
        node->tag = code;
    }
    std::function<void(const tree*,int)> dump = [&]( const tree* node, int space ) {
        for( auto [key,sub] : node->sub ) {
            for( int i = 0; i < space; i++ ) std::cout << " ";
            std::cout << key;
            if( sub.tag ) std::cout << " " << sub.tag;
            std::cout << std::endl;
            dump(&sub, space+1);
        }
    };
    // dump(root,0);
}

int tree::reach( tree* begin, char* str ) {
    if( !begin or !str ) return 0;
    if( str[0] == '\0' ) return begin->tag;
    if( begin->sub.count(str[0]) ) return reach(&begin->sub[str[0]], str+1);
    return 0;
}

}

#endif