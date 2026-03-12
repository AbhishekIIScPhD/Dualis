#include "../../SeaImpl/TokenBucketImpl.h"

#include <stdio.h>

extern int nd();

int main(int argc, char* argv[]) {
    TokenBucket tb;
    int b_size, c_rate, avai_tokens = 0, consumed_tokens = 0;;

    // Non-deterministic initialization with constraints
    b_size = nd();
    c_rate = nd();
    
    __VERIFIER_assume(b_size > 0);
    __VERIFIER_assume(c_rate > 0);
    __VERIFIER_assume(b_size >= c_rate);

    tb.generateTokens(b_size);
    avai_tokens = tb.getAvailableTokens();

    while (avai_tokens >= c_rate) {
        tb.consume(c_rate);
        avai_tokens = tb.getAvailableTokens();
        consumed_tokens += c_rate;
    }

    sassert(consumed_tokens >= c_rate);
    return 0;
}
