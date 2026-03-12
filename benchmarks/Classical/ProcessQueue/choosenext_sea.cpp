#include "../../SeaImpl/ProcessQueueImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  ProcessQueue pq;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    pq.insert(v);
  }

  int min_ttw = pq.minttw();
  int len = pq.len();
  pq.choosenext();
  int min_ttw1 = pq.minttw();
  int len1 = pq.len();
  bool expr = (false);

  sassert(expr);

  return 0;
}
