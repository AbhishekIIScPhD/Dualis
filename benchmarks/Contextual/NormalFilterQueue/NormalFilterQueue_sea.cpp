#include "../../SeaImpl/NormalFilterQueueImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  NormalFilterQueue nfq;
  int N = nd(), maxPrio = 0, minPacketSize = 500, maxPrio1, minPacketSize1, len = 0;

  __VERIFIER_assume(N > 0);

  for (int i = 0; i < N; ++i) {

    int packetSize = nd();
    int prio = nd() % 2;
    
    if (packetSize > 0) {
      if (prio == 0 && packetSize >= 500) {
	printf("prio %d packetSize %d\n", prio, packetSize);

	nfq.append(std::make_pair(prio, packetSize));

	len = nfq.len();
	maxPrio = nfq.maxPrio();
	minPacketSize = nfq.minPacketSize();
	printf("len %d maxPrio %d minPacketSize %d\n", len, maxPrio , minPacketSize);
      }
    }
  }
 
  nfq.processQueue();

  len = nfq.len();
  maxPrio1 = nfq.maxPrio();
  minPacketSize1 = nfq.minPacketSize();

  printf("maxPrio %d maxPrio1 %d minPacketSize %d minPacketSize1 %d\n", maxPrio, maxPrio1 , minPacketSize, minPacketSize1);

  sassert(len!= 0 or maxPrio == maxPrio1 and minPacketSize == minPacketSize1);
  return 0;
}
