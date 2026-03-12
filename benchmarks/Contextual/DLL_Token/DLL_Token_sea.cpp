#include "../../SeaImpl/DLL_TokenImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    DLLTok dt;
    int N = nd(), len = 0, min = MAX, max = MIN;

    __VERIFIER_assume(N >= 0);

    for (int i = 0; i < N; ++i) {
        int val = 0;
        
        dt.push(val);
        
        min = dt.min();
        len = dt.len();
        max = dt.max();
    }

    sassert((len == 0) or (min == 0 and max == 0));

    return 0;
}
