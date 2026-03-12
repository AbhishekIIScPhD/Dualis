#include "../../SeaImpl/SkipListImpl.h"

#include <stdio.h>

extern int nd();

#undef MAX
#define MAX 100

int main(int argc, char* argv[]) {
    SkipList sl;
    int N = nd(), isPresent, min, max, min1, max1, len1, len = 0;

    __VERIFIER_assume(N > 0);

    // Insert N values
    for (int i = 0; i < N; i++) {
        int v = i;
        
        sl.insert(v);

	printf("v %d, len %d\n", v, len);

        isPresent = sl.isPresent(v);
        min = sl.min();
        max = sl.max();
        len = sl.len();
	printf("v %d, len %d\n", v, len);
    }

    int k = nd();
    __VERIFIER_assume(k >= 0 && k < N);
    printf("k %d, len %d\n", k, len);
    len = sl.len();
    isPresent = sl.isPresent(k);
    int ret1 = sl.remove(k);
    len1 = sl.len();
    printf("k %d, len %d len1 %d ret1 %d \n", k, len, len1, ret1);

    sassert(len1 == (len - 1));

    return 0;
}
