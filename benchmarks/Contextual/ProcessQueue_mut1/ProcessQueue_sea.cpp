#include "../../SeaImpl/ProcessQueueImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    ProcessQueue pq;
    int N = nd(), len = 0, min_ttw = MAX, nexttime = 1;
    __VERIFIER_assume(N > 0 && N < MAX_PROC);
    
    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; ++i) {
        int nondet = nd();
        
        if (nondet % 2 == 0) {
            pq.insert(nexttime);
            nexttime++;
        } else if (nexttime > 1) {
            pq.choosenext();
            nexttime--;
        }
    }
    len = pq.len();
    min_ttw = pq.minttw();
    
    sassert(len <= 0 or min_ttw >= 1);
    return 0;
}
