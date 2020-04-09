#ifndef __alex_code_lexical_hpp_cpp__
#define __alex_code_lexical_hpp_cpp__

#include "alex.internal.hpp"

namespace alex {

const std::string code_token_hpp = R"...(
#ifndef __token__
#define __token__

#include <string>

namespace %l {

/**
 * @struct token : 词法记号结构体 */
struct token {

    public:

        /**
         * @member id : 记号ID */
        int id;

        /**
         * @member bl : 起始行 */
        int bl;
        
        /**
         * @member bc : 起始列 */
        int bc;

        /**
         * @member el : 终止行 */
        int el;

        /**
         * @member ec : 终止列 */
        int ec;

        /**
         * @member tx : 文本内容 */
        std::string tx;
};

}

#endif
)...";

}

#endif