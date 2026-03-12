#include "../../SeaImpl/BinaryTreeImpl.h"

extern int nd();

int main(int argc, char* argv[])
{

  BinaryTree bt;
  int N;

  __VERIFIER_assume( N >= 0);

  for (int i = 0; i < N; i++){
    int v = nd();
    bt.insert(v);
  }

  int min = bt.min();
  int isEmpty = bt.isEmpty();
  int n = nd();
  bt.insert(n);
  int min1 = bt.min();
  int isEmpty1 = bt.isEmpty();
  bool expr = (((((min - min1) <= 0) or (((n - min1) <= 0) and ((min - min1) > 0))) and (isEmpty <= 0)) or (((n - min1) <= 0) and (isEmpty > 0)));

  sassert(expr);

  return 0;
}
