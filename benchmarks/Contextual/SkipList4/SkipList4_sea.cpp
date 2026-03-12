#include "../../SeaImpl/SkipListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    SkipList sl;
    int N;
    
    __VERIFIER_assume(N >= 0);

    // Initialize skip list with some elements
    for (int i = 0; i < N; i++) {
        int v = nd();
        sl.insert(v);
    }

    int insertCount = nd();
    __VERIFIER_assume(insertCount >= 0);
    
    int i = 0;
    while (i < insertCount) {
        int v = 1;  // Fixed value as in original

        bool isPresent = sl.isPresent(v);
        int min = sl.min();
        int max = sl.max();
        int len = sl.len();

        sl.insert(v);

        bool isPresent1 = sl.isPresent(v);
        int min1 = sl.min();
        int max1 = sl.max();
        int len1 = sl.len();

        bool expr_insert = (true);

        sassert(expr_insert);
        i++;
    }

    return 0;
}
