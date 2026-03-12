#include "../../SeaImpl/AtomicHashMapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    HashTable ahm;
    int N = nd();
    int len, min, max, containsk;
    
    __VERIFIER_assume(N >= 1);

    // Insert N elements using insert method
    int i = 0;
    while (i < N) {
        int k = i;
        int v = i;

        ahm.insert(k, v);

        len = ahm.len();
        min = ahm.minKey();
        max = ahm.maxKey();
        containsk = ahm.contains(k);

        i++;
    }
    // Insert N elements using insert1 method
    i = 0;
    while (i < N) {
        int k = i;
        int v = i;

        ahm.insert1(k, v);

        len = ahm.len();
        min = ahm.minKey();
        max = ahm.maxKey();
        containsk = ahm.contains(k);

        i++;
    }

    int ret = ahm.find(k);
    sassert(ret == max);
    return 0;
}
