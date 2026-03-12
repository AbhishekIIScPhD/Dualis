#include "../../SeaImpl/BinaryTreeImpl.h"

extern int nd();

// This should be defined in your BinaryTreeImpl.h
// Make sure it's available here, e.g., #define MAX_N 10
// Or just know what the bound is.

int main(int argc, char* argv[])
{

  BinaryTree bt;
  int N;

  __VERIFIER_assume( N >= 0);
  
  // --- THIS IS THE KEY CHANGE ---
  // We are inserting N items, and then ONE MORE (n).
  // We must ensure N+1 items fit in the array.
  // So, N must be *less than* MAX_N.
  __VERIFIER_assume( N < MAX_N ); 

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
  
  // This is a great assertion for a min-heap!
  // It correctly states that the new min (min1) is either:
  // 1. The same as the old min (if n >= min)
  // 2. Is equal to n (if n < min)
  bool expr = (((((min - min1) <= 0) or (((n - min1) <= 0) and ((min - min1) > 0))) and (isEmpty <= 0)) or (((n - min1) <= 0) and (isEmpty > 0)));

  sassert(expr);

  return 0;
}
