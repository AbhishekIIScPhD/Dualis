#include "../../SeaImpl/MinImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    Min ml;
    int N = nd(), lmin = MAX, gmin = MAX;

    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; ++i) {
        int v = nd();
         ml.append(v);
        
        lmin = ml.minElem();
        
        if (gmin > v) {
            gmin = v;
        }
    }

    sassert(lmin == gmin);
    return 0;
}
