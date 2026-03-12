#include "../../SeaImpl/CalenderImpl.h"
#include <stdio.h>

extern int nd();

int main(int argc, char* argv[]) {
    Cal cal;
    int N=nd(), len = 0, maxDiff = MIN;

    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; ++i) {
        int ev1 = nd();
        int ev2 = nd();
	

	printf("ev1 : %d, ev2 : %d\n", ev1, ev2);
        if (((ev1 >= 0 && ev1 <= 3) && (ev2 >= 0 && ev2 <= 3)) &&
            ((ev1 - ev2) < 2)) {
            
            cal.insert(std::make_pair(ev1, ev2));
            
            len = cal.len();
            maxDiff = cal.maxDiff();
	    printf("len : %d, maxDiff : %d\n", len, maxDiff);
        }
    }
    printf("len : %d, maxDiff : %d\n", len, maxDiff);
    sassert(len <= 0 or maxDiff < 2);
    return 0;
}
