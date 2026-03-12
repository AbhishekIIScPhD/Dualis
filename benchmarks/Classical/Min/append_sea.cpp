#include "../../SeaImpl/MinImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  Min mn;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    mn.append(v);
  }

  int lmin = mn.minElem();
  int v = nd();
  mn.append(v);
  int lmin1 = mn.minElem();
  bool expr = (false);

  sassert(expr);

  return 0;
}
