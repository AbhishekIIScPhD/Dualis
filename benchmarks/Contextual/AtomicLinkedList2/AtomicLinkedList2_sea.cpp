#include "../../SeaImpl/AtomicLinkedListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    AtomicLinkedList all;
    int N = nd(), len = 0;

    __VERIFIER_assume(N >= 1);

    int k = 0;
    while (k < N) {
      all.insertHead(k);
      len = all.len();
      k++;
    }

    int items_processed = 0;
    int ret = MAX;
    int max = MIN;

    while (len != 0) {
      max = all.max();
      ret = all.popHead();
      len = all.len();
      items_processed++;
    }

    sassert(ret == max);
    return 0;
}
