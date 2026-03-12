#include "../../SeaImpl/SkipListImpl.h"

#include <stdio.h>

extern int nd();

int main(int argc, char* argv[]) {
    SkipList sl;
    int N = nd(), len = 0;

    __VERIFIER_assume(N > 0);

    // Insert N values
    for (int i = 0; i < N; i++) {
        int v = i;

        sl.insert(v);

        bool isPresent = sl.isPresent(v);
        len = sl.len();
	printf("v %d, len %d\n", v, len);
    }

    // Test lower_bound operation
    int k = nd();
    __VERIFIER_assume(k >= 0 && k < N);
    int lb_ret1 = sl.lower_bound(k);

    printf("k %d lb_ret1 %d len %d\n", k, lb_ret1, len);
    sassert(lb_ret1 >= k);

    return 0;
}
