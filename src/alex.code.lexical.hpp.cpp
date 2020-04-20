#ifndef __alex_code_lexical_hpp_cpp__
#define __alex_code_lexical_hpp_cpp__

#include "alex.internal.hpp"

namespace alex {

const std::string code_lexical_hpp = R"...(
#ifndef __lexical__
#define __lexical__

#include <istream>
#include <functional>
#include "token.hpp"

namespace %l {

/**
 * @class LexicalContext : 词法分析上下文 */
class LexicalContext {

    private:

        /**
         * @member m_cur : 当前正在分析的符号 */
        token m_cur;

        /**
         * @member m_line, member_column : 当前的行列 */
        int m_line, m_column;

        /**
         * @member m_offset : 当前所在的偏移量 */
        int m_offset;

        /**
         * @member m_state : 当前的状态 */
        int m_state;

        /**
         * @member m_pre : 当前预览 */
        int m_pre;

        /**
         * @member m_is : 当前输入流指针 */
        std::istream* m_is;

    private:

        /**
         * @method _goto : 转入指令
         * @desc :
         *  保留当前输入不作处理，切换状态，让另一个状态来处理这个输入
         * @param state : 目标状态号 */
        void _goto( int state );

        /**
         * @method _into : 进入指令
         * @desc :
         *  将当前输入追加到当前符号，进入下一个状态
         * @param state : 目标状态号 */
        void _into( int state );

        /**
         * @method _accept : 接受指
         * @desc :
         *  接受一个词法符号，并进入新的状态
         * @param state : 目标状态号
         * @param id : 接受的符号ID
         * @param take : 是否将当前输入追加到符号 */
        void accept( int state, int id, bool take );

    public:

        /**
         * @member ontoken : 符号回调
         * @desc :
         *  当状态机成功分析了一个符号时调用，失败时也会调用，传入失败符号
         * @param <0> : 成功接受或失败接受的符号
         * @return bool : 返回false时停止分析 */
        std::function<bool(token&&)> ontoken;
    
    public:

        LexicalContext() = default;
        ~LexicalContext() = default;

        /**
         * @method perform : 执行分析
         * @desc :
         *  执行词法分析，若设置了符号回调，每次成功或失败分析一个符号都会调用回调
         *  若输入流尚未准备好则失败，在输入流中读取到\0或EOF时停止检查
         * @param source : 源代码的输入流
         * @return int : 返回分析的字节数，不包括结束符 */
        int perform( std::istream& source );
};

}

#endif
)...";

}

#endif