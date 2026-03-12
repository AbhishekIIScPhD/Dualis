#include "../../SeaImpl/BlueWhiteImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  BWList bwl;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    bwl.push(v);
  }

  int bcount = bwl.blue_count();
  int color = nd();
  bwl.push(color);
  int bcount1 = bwl.blue_count();
  bool expr = (false);

  sassert(expr);

  return 0;
}
