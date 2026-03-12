#include "../../SeaImpl/BinaryHeapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    BinaryHeap bh;
    int N = nd(), ret, len = 0, isHeap = 1;

    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; i++) {
        int val = nd();
        bh.insert(val);
    }

    ret = bh.deleteMin();

    len = bh.len();
    isHeap = bh.isHeap();

    bh.downHeap();
    isHeap = bh.isHeap();

    sassert(isHeap == 1);

    return 0;
}
