#include "../../SeaImpl/BinaryTreeImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  BinaryTree bt;
  int N;
 
  __VERIFIER_assume( N >= 0);
 __VERIFIER_assume( N <= MAX_N); 

  for (int i = 0; i < N; i++){
    int v = nd();
    bt.insert(v);
  }

  int min = bt.min();
  int isEmpty = bt.isEmpty();
  int v = nd();
  int ret1 = bt.search(v);
  bool expr = (((ret1 <= 0) or (((min - v) <= 0) and (ret1 > 0))) and (ret1 > (- 1)));
  sassert(expr);
  return 0;
}
