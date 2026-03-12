#include "../../SeaImpl/AtomicHashMapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    HashTable ahm;
    int N = nd();
    int len1, min1, max1;
    
    __VERIFIER_assume(N >= 0);

    int i = 0;
    while (i < N) {
        int k = i;
        int v = i;
        
        ahm.insert(k, v);

        len1 = ahm.len();
        min1 = ahm.minKey();
        max1 = ahm.maxKey();
        containsk1 = ahm.contains(v);

        i++;
    }

    i = 0;
    while (i < N) {
        int k = i;
        int v = i;

        ahm.insert1(k, v);

        len1 = ahm.len();
        min1 = ahm.minKey();
        max1 = ahm.maxKey();
        containsk1 = ahm.contains(v);

        i++;
    }

    sassert(len == N);
    return 0;
}
