#include "../../SeaImpl/CalenderImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  Cal cal;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int ev1 = nd();
    int ev2 = nd();
    auto p = make_pair(ev1, ev2);
    cal.insert(p);
  }

  int len = cal.len();
  int maxDiff = cal.maxDiff();
  int ev1 = nd();
  int ev2 = nd();
  auto p = make_pair(ev1, ev2);
  cal.insert(p);
  int len1 = cal.len();
  int maxDiff1 = cal.maxDiff();
  bool expr = (false);

  sassert(expr);

  return 0;
}
