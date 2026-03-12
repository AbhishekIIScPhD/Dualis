#include "../../SeaImpl/NormalFilterQueueImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  NormalFilterQueue obj;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v1 = nd();
    int v2 = nd();
    obj.insert(v1, v2); // Assuming insert takes priority and packet size
  }

  int len = obj.len();
  int minPacketSize = obj.minPacketSize();
  int maxPrio = obj.maxPrio();
  obj.processQueue();
  int len1 = obj.len();
  int minPacketSize1 = obj.minPacketSize();
  int maxPrio1 = obj.maxPrio();
  bool expr = (false);

  sassert(expr);

  return 0;
}
