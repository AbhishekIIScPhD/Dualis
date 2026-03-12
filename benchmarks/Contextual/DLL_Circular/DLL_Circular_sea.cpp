#include "../../SeaImpl/DLL_CircularImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    DLLCircular dlc;
    int N = nd(), max = 0, val = 0;
    
    __VERIFIER_assume(N > 0);
    
    for (int i = 0; i < N; ++i) {
        bool condition = nd();
        if (condition) {
            if (val < 3) {
                val = val + 1;
            }
        }

        dlc.push(val);
        max = dlc.max();
    }

    sassert( max >= 0 && max <= 3);

    return 0;
}
