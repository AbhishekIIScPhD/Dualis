#include "../../SeaImpl/BinaryTreeImpl.h"

extern int nd();

int main(int argc, char* argv[]) {

  BinaryTree bt;
  int N = nd();

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; ++i) {
    int n = nd();
    if (n >= 0) {
      bt.insert(n);
    }
  }

  int v = nd();
  __VERIFIER_assume(v < 0);
  bool ret1 = bt.search(v);

  sassert(!(ret1 == 1));
  return 0;
}
