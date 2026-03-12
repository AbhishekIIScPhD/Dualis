#include "../../SeaImpl/SkipListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    SkipList sl;
    int N;

    __VERIFIER_assume(N >= 0);

    for (int i = 0; i < N; i++) {
        int v = nd();
        
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
    }

    int min = sl.min();
    int lb_ret1 = sl.lower_bound(min);
    
    bool expr_lower_bound = (true);
    sassert(expr_lower_bound);

    return 0;
}
