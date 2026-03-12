#include "../../SeaImpl/NormalFilterQueueImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  NormalFilterQueue obj;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int prio = nd();
    int packetSize = nd();
    obj.append(make_pair(prio, packetSize));
  }

  int minPacketSize = obj.minPacketSize();
  int maxPrio = obj.maxPrio();
  int len = obj.len();
  int prio = 0;
  int packetSize = nd();
  obj.append(make_pair(prio, packetSize));
  int minPacketSize1 = obj.minPacketSize();
  int maxPrio1 = obj.maxPrio();
  int len1 = obj.len();
  bool expr = (false);

  sassert(expr);

  return 0;
}
