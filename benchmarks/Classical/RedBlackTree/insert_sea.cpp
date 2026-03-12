#include "../../SeaImpl/RedBlackTreeImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  RedBlackTree rbt;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    rbt.insert(v);
  }

  int max = rbt.max();
  int min = rbt.min();
  int len = rbt.len();
  int noDup = rbt.noDup();
  int val = nd();
  rbt.insert(val);
  int max1 = rbt.max();
  int min1 = rbt.min();
  int len1 = rbt.len();
  int noDup1 = rbt.noDup();
  bool expr = (false);

  sassert(expr);

  return 0;
}
