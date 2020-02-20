#ifndef __alex_internal__
#define __alex_internal__

#include <functional>
#include <string>
#include <map>
#include "jsonz.hpp"
#include "alex.hpp"

namespace alex {

using fsm_instruction = std::tuple<int,chainz<json>>;
using fsm_program = chainz<fsm_instruction>;
using fsm_state = std::map<int, fsm_program>;

struct tree {
    int tag = 0;
    std::map<int,struct tree> sub;

    static int reach( tree*, char* );
};
extern tree* root;

/**
 * @struct state_frame : 状态帧
 * @desc :
 *  状态帧用于管理一个状态的局部参量
 */
struct state_frame {
    int state;
    tokens cache;
};

/**
 * @class vstack : 虚拟栈
 * @desc :
 *  运行alex状态程序时所使用的状态栈。
 *  内部包含了所有alex状态程序可以访问的内容。
 */
struct vstack {

    /**
     * @member var : 变量空间
     * @desc : 包含了所有的具名变量 */
    std::map<std::string,json>& var;

    /**
     * @member mem : 内存空间
     * @desc : 一个基于字节寻址的内存空间 */
    char*& mem;

    /**
     * @member memlen : 内存空间长度
     * @desc : 标注内存空间的长度 */
    int& memlen;

    /**
     * @member state : 当前状态
     * @desc : 当前和目标的状态号 */
    int& state;

    /**
     * @member os : 输出流
     * @desc : 状态程序可以访问的输出流 */
    std::ostream& os;

    enum vaction { 
        terminate, 
        drop, input, pause, accept,
        movin, movout, redu, stay } 
    action;

    int c;

    int n;

    int s;

    void autowire( token pre, token buf, chainz<state_frame>& states );
};

struct command_desc {
    std::string name;
    std::string args;
    std::function<std::string(vstack&, const chainz<json>&)> proc;

    static void init();
};
extern const std::map<int, command_desc> commands;

}

#endif