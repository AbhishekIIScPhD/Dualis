#include "../../SeaImpl/TokenBucketImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    TokenBucket tb;
    int b_size, c_rate;
    
    // Non-deterministic initialization with constraints
    b_size = nd();
    c_rate = nd();
    
    __VERIFIER_assume(b_size > 0);
    __VERIFIER_assume(c_rate > 0);
    __VERIFIER_assume(b_size >= c_rate);

    tb.generateTokens(b_size);
    int avai_tokens = tb.getAvailableTokens();

    int consumed_tokens = 0;
    avai_tokens = tb.getAvailableTokens();

    while (avai_tokens >= c_rate) {
        tb.consume(c_rate);
        int avai_tokens1 = tb.getAvailableTokens();
	consumed_tokens += c_rate;
        avai_tokens = avai_tokens1;
    }

    sassert(consumed_tokens <= b_size);
    return 0;
}
