#ifndef __alex__
#define __alex__

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <list>
#include <set>
#include "jsonz.hpp"

namespace alex {

/**
 * @strct token : 词法符号
 * @desc :
 *  存储一个词法符号的所有相关信息
 */
struct token {
    
    /** @member id : 符号ID */
    int id;
    
    /** @member begl : 起始行 */
    int begl;

    /** @member begc : 起始列 */
    int begc;

    /** @member endl : 结束行 */
    int endl;

    /** @member endc : 结束列 */
    int endc;

    /** @member tx : 书写内容 */
    std::string tx;

    /** [TODO] 将存储形式修改成为offset:length形式 */

    void clear();
};
using tokens = chainz<token>;

/**
 * @class diagram : 有穷状态机状态图
 * @desc :
 *  有穷状态机状态图用于描述一个有穷状态机在不同状态下应对不同输入时应当采取的一系列动作。
 */
class fsm : public std::map<int,std::map<int,chainz<std::tuple<int,chainz<json>>>>> {

    public:

        /**
         * @static-method compile : 从文本编译
         * @desc :
         *  从文本编译alex状态机文法到状态图。
         *  从文本的开始到结束扫描所有的内容。
         */
        static fsm compile( std::istream& );

        /**
         * @static-method load : 加载
         * @desc :
         *  从二进制文件数据流加载有穷状态机状态跳转图
         */
        static fsm load( std::istream& );

        /**
         * @method print : 打印状态
         */
        static std::string print( fsm::value_type::second_type& );

        /**
         * @method print : 打印程序
         */
        static std::string print( fsm::value_type::second_type::value_type::second_type& );

        /**
         * @method print : 打印
         * @desc :
         *  将有穷状态机打印到输出流
         */
        bool print( std::ostream& );

        /**
         * @method genstate : 产生一个新的状态
         * @desc :
         *  产生一个不重复的状态，但是无法确保此状态是否已经被引用了。
         */
        int genstate();

        /**
         * @method findexit: 寻找出度路径
         * @desc :
         *  寻找某状态下一个输入对应的出度路径
         * @param state : 状态
         * @param input : 输入
         * @return std::tuple<int,chainz<json>>* : 返回出度指令的指针
         */
        std::tuple<int,chainz<json>>* findexit( int state, int input );

        /**
         * @method optimize : 优化
         * @desc :
         *  将过渡状态省略，删除没有入度的状态
         * @return int : 返回删除的状态个数
         */
        int optimize();

        /**
         * @method forward : 快进
         * @desc :
         *  若当前状态是中转状态，则快进到中转链的目的状态 */
        int forword( int state );
};
std::ostream& operator << (std::ostream&, fsm& );

/**
 * @struct regex : Alex正则表达式
 * @desc :
 *  Alex正则表达式用于在Alex词法规则中描述词法。
 *  Alex正则表达式可以被转化为Alex有穷状态机跳转图。
 */
struct regex {

    public:
        /**
         * @enum type_t : 正则表达式类型
         * @desc : 描述正则表达式的类型 
         */
        enum class type_t {
            /** @character: 字符 */
            character,
            /** @family: 字符族 */
            family,
            /** @optional: 可选 */
            optional,
            /** @any: 任意次 */
            any,
            /** @more: 一或多次 */
            more,
            /** @sequence: 序列 */
            sequence,
            /** @reverse: 反向字符集 */
            reverse,
            /** @collection: 字符集 */
            collection,
            /** @options: 多重选项 */
            options,
            /** @range: 范围 */
            range,
            /** @none: 空表达式 */
            none,
        };

        /**
         * @enum family_t : 字符族
         * @desc : 用于描述某一族字符
         */
        enum class family_t { 
            /** @any: 任意字符 */
            any,
            /** @space: 空白符 */
            space,
            /** @digit: 数字 */
            digit,
            /** @punct: 标点 */
            punct,
            /** @lower: 小写字母 */
            lower,
            /** @upper: 大写字母 */
            upper,
            /** @boundary: 边界 */
            boundary,
        };
    
    public:

    /** @member content: 表达式内容 */
    chainz<regex> content;

    /** @member value: 表达式值 */
    int value;

    /** @member type: 表达式类型 */
    type_t type;

    private:

        /**
         * @function extract : 提取
         * @desc :
         *  从输入流提取一个最小的单位 */
        static regex extract( std::istream&, bool inner = false );
    
    public:

        /** 
         * @static-method compile: 从文本编译表达式
         * @desc :
         *  从文本编译正则表达式，正则表达式应当从流的第一个字符开始。
         *  正则表达式到第一个‘/’而结束。
         *  编译过程不会消耗掉用于结束正则文法的字符。
         */
        static regex compile( std::istream&, char terminator = '/' );

        /**
         * @method attach : 绑定
         * @desc :
         *  将正则表达式绑定到有限状态机
         * @param machine : 有限状态机
         * @param start : 起始状态号
         * @param accept : 接受记号
         * @return : 返回接受记号时的状态号
         */
        std::tuple<int,std::set<int>> attach( fsm& machine, int start, int accept );

        /**
         * @method attach : 绑定
         * @desc :
         *  将正则表达式绑定到有限状态机
         * @param machine : 有限状态机
         * @param start : 起始状态号
         * @return : 返回子式最后到达的状态
         */
        std::tuple<int,std::set<int>> attach( fsm& machine, int start );

        void print( std::ostream& );
};
std::ostream& operator << ( std::ostream&, regex& );

/**
 * @struct lexi : 词法规则
 * @desc :
 *  一条词法规则描述了如何从输入流中匹配一个词汇
 *  以及词汇拥有怎样的后缀时才能被接受
 *  一条正确的词汇最好以正数为id
 *  词汇不能要求负数id词汇作为前缀，因为负数前缀表示此前缀可选
 */
struct lexi {

    /** @member name : 符号名称 */
    std::string name;

    /** @member match : 匹配规则 */
    regex match;

    /** @member suffix : 后缀规则,如果后缀规则被满足，将接受为错误记号 */
    regex suffix;
};
std::ostream& operator << ( std::ostream&, lexi& );

/**
 * @struct lex : 词法规则集
 * @desc :
 *  词法规则集用于描述一组词法，表示一种语言的所有词法规则
 */
class lex : public std::map<int,lexi> {

    public:
        /** 
         * @method compile : 编译
         * @desc :
         *  将词法源码编译
         */
        static lex compile( std::istream& );

        /**
         * @method compile : 编译
         * @desc :
         *  将词法规则编译成为有限状态机
         */
        fsm compile();

        /**
         * @method gencpp : 产生CPP代码
         * @desc :
         *  产生用于分析词法的CPP代码
         * @param lang : 语言名称
         */
        std::map<std::string,std::string> gencpp( const std::string& lang );

        /**
         * @method genvtd : 产生词汇表定义
         * @desc :
         *  产生终结词汇表定义
         * @param lang : 语言名称
         */
        std::string genvtd( const std::string& lang );

        /**
         * @method gentokend : 产生词法符号定义
         * @desc :
         *  产生词法符号定义
         * @param lang : 语言名称
         */
        std::string gentokend( const std::string& lang );

        /**
         * @method genctxd : 产生词法分析上下文定义
         * @desc :
         *  产生词法分析上下文定义
         * @param lang : 语言名称
         */
        std::string genctxd( const std::string& lang );

        /**
         * @method genctxi : 产生词法上下文实现
         * @desc :
         *  产生词法上下文实现
         * @param lang : 语言名称
         */
        std::string genctxi( const std::string& lang );
};
std::ostream& operator << ( std::ostream&, lex& );

/**
 * @class context : 上下文环境
 * @desc :
 *  上线文环境用于运行有限状态机
 */
class context {

    private:
        const fsm::value_type::second_type* state = nullptr;
        const fsm& diag;

        std::map<std::string,json> var;
        char* mem = nullptr;
        int memlen = 0;

        int line, column;
        token buf;

    private:
        bool initiate();

    public:
        context( const alex::fsm& diagram );
        
        tokens perform( std::istream&, std::ostream& );

        tokens perform( tokens, std::ostream& );

        std::string printlex();
};

}

#endif