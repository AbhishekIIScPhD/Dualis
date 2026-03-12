#include "../../SeaImpl/MaxImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  Max mx;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    mx.append(v);
  }

  int lmax = mx.maxElem();
  int v = nd();
  mx.append(v);
  int lmax1 = mx.maxElem();
  bool expr = (false);

  sassert(expr);

  return 0;
}
