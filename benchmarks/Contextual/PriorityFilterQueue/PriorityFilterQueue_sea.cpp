#include "../../SeaImpl/PriorityFilterQueueImpl.h"

#include <stdio.h>

extern int nd();

int main(int argc, char* argv[]) {
    PriorityFilterQueue pfq;
    int N = nd(), minPrio = 1, maxPacketSize = 499, len = 0, minPrio1 = 1, maxPacketSize1 = 499;

    __VERIFIER_assume(N > 0);

    printf("before loop");
    for (int i = 0; i < N; i++) {
        int packetSize = nd();
        int prio = nd();
        
        if (packetSize > 0) {
            if (prio == 1 && packetSize < 500) {
        	printf("prio %d packetSize %d\n", prio, packetSize);
		pfq.append(std::make_pair(prio, packetSize));
            }
        }
    }
    pfq.processQueue();

    len = pfq.len();
    minPrio1 = pfq.minPrio();
    maxPacketSize1 = pfq.maxPacketSize();

    // printf("minPrio %d minPrio1 %d maxPacketSize %d maxPacketSize1 %d\n", minPrio, minPrio1 , maxPacketSize, maxPacketSize1);

    sassert(len != 0 or (minPrio1 == minPrio && maxPacketSize1 == maxPacketSize));

    return 0;
}
