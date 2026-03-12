#include "../../SeaImpl/BinaryHeapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    BinaryHeap bh;
    int N = nd(), isHeap = 1, len = 0, ret = nd();

    __VERIFIER_assume(N > 0);

    int i = 0;
    while (i < N) {
        bh.insert(i);
        
        isHeap = bh.isHeap();
        len = bh.len();
        i++;
    }

    int d = 0;

    while(bh.len() != 0) {
        ret = bh.deleteMin();
        len = bh.len();
        isHeap = bh.isHeap();

        bh.downHeap();
        isHeap = bh.isHeap();
        d++;
    }

    sassert(ret == (N - 1) and isHeap == 1);
    return 0;
}
