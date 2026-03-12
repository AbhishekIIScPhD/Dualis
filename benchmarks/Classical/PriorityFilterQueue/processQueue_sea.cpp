#include "../../SeaImpl/PriorityFilterQueueImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  PriorityFilterQueue obj;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int priority = nd();
    int packetSize = nd();
    obj.insert(priority, packetSize);
  }

  int len = obj.len();
  int maxPacketSize = obj.maxPacketSize();
  int minPrio = obj.minPrio();
  obj.processQueue();
  int len1 = obj.len();
  int maxPacketSize1 = obj.maxPacketSize();
  int minPrio1 = obj.minPrio();
  bool expr = (false);

  sassert(expr);

  return 0;
}
