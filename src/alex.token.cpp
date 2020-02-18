#ifndef __alex_token_cpp__
#define __alex_token_cpp__

#include "alex.internal.hpp"

namespace alex {

void token::clear() {
    id = begl = begc = endl = endc = -1;
    tx.clear();
}

}

#endif