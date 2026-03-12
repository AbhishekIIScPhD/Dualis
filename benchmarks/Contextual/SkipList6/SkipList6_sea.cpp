#include "../../SeaImpl/SkipListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    SkipList sl;
    int N;
    
    __VERIFIER_assume(N >= 0);

    int i = 0;
    while (i < N) {
        int v = 1;
        
        bool isPresent = sl.isPresent(v);
        int min = sl.min();
        int max = sl.max();
        int len = sl.len();

        sl.insert(v);

        bool isPresent1 = sl.isPresent(v);
        int min1 = sl.min();
        int max1 = sl.max();
        int len1 = sl.len();

        bool expr_insert = (((max <= (- 12)) and (isPresent <= 0)) or (isPresent > 0));

        sassert(expr_insert);
        i++;
    }

    int k = 1;
    int len = sl.len();

    bool ret1 = sl.remove(k);

    int len1 = sl.len();

    bool expr_remove = (true);

    sassert(expr_remove);

    return 0;
}
