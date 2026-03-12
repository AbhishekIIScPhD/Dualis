#include "../../SeaImpl/PriorityFilterQueueImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  PriorityFilterQueue obj;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int prio = nd();
    int packetSize = nd();
    obj.append(make_pair(prio, packetSize));
  }

  int maxPacketSize = obj.maxPacketSize();
  int minPrio = obj.minPrio();
  int len = obj.len();
  int prio = 1;
  int packetSize = nd();
  obj.append(make_pair(prio, packetSize));
  int maxPacketSize1 = obj.maxPacketSize();
  int minPrio1 = obj.minPrio();
  int len1 = obj.len();
  bool expr = (false);

  sassert(expr);

  return 0;
}
