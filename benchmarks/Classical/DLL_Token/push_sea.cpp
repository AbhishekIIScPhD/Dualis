#include "../../SeaImpl/DLL_TokenImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  DLLTok dllt;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    dllt.push(v);
  }

  int min = dllt.min();
  int max = dllt.max();
  int val = nd();
  int len = dllt.len();
  dllt.push(val);
  int min1 = dllt.min();
  int max1 = dllt.max();
  int len1 = dllt.len();
  bool expr = (false);

  sassert(expr);

  return 0;
}
