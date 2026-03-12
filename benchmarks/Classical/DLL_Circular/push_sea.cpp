#include "../../SeaImpl/DLL_CircularImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  DLLCircular dllc;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    dllc.push(v);
  }

  int max = dllc.max();
  int val = nd();
  dllc.push(val);
  int max1 = dllc.max();
  bool expr = (false);

  sassert(expr);

  return 0;
}
