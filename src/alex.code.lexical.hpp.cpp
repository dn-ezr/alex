#ifndef __alex_code_lexical_hpp_cpp__
#define __alex_code_lexical_hpp_cpp__

#include "alex.internal.hpp"

namespace alex {

const std::string code_lexical_hpp = R"...(
#ifndef __lexical__
#define __lexical__

#include "token.hpp"

namespace %l {

class LexicalContext {

};

}

#endif
)...";

}

#endif