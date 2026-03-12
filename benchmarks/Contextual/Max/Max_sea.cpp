#include "../../SeaImpl/MaxImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    Max ml;
    int N = nd(), len = 0, lmax = MIN, gmax =  MIN;

    __VERIFIER_assume(N > 0);

    
    for (int i = 0; i < N; ++i) {
        int v = nd();
        
        ml.append(v);
        
        len = ml.len();
        lmax = ml.maxElem();

        if (gmax < v) {
            gmax = v;
        }
    }

    sassert(gmax == lmax);

    return 0;
}
