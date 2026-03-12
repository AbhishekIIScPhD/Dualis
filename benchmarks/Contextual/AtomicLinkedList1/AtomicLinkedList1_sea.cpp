#include "../../SeaImpl/AtomicLinkedListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    AtomicLinkedList all;
    int N = nd(), len = 0;
    int items_processed = 0;
    
    __VERIFIER_assume(N > 0);
    
    // Insert N items using non-deterministic values
    int k = 0;
    while (k < N) {
        int value = nd();
        all.insertHead(value);
        len = all.len();
        k++;
    }

    len = all.len();
    while (len > 0) {
        int ret1 = all.popHead();
        len = all.len();
        items_processed++;
    }

    sassert(items_processed == N);

    return 0;
}
