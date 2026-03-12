#include "../../SeaImpl/RedBlackTreeImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    RedBlackTree rbt;
    int N = nd();
    
    __VERIFIER_assume(N > 0);

    int i = 0;
    while (i < N) {

        rbt.insert(i);

        i = i + 1;
    }

    int data = nd();

    __VERIFIER_assume(data >= 0 and data < N);
    int ret1 = rbt.search(data);
    

    sassert(ret1 == 1);
    return 0;
}
