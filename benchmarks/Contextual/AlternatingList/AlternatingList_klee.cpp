#include "../../SeaImpl/AlternatingListImpl.h"
#include <klee/klee.h>

int main() {
  AlternatingList al;
  int N;
  klee_make_symbolic(&N, sizeof(N), "N");
  klee_assume(N >= 0 && N <= 5);   // bounded

  // Initialize the list with some elements
  for (int i = 0; i < N; i++) {
    int v;
    klee_make_symbolic(&v, sizeof(v), "v");
    klee_assume(v >= 0 && v <= 10); // restrict values
    if (i % 2 == 0) {
      al.push(v);
    } else {
      al.push1(v);
    }
  }

  int flag = 1;
  int loop_count;
  klee_make_symbolic(&loop_count, sizeof(loop_count), "loop_count");
  klee_assume(loop_count >= 0 && loop_count <= 5); // bounded
  int top = 0;
  int len = 0;

  for (int i = 0; i < loop_count; ++i) {
    int val = 0;
    if (flag) {
      val = 1;
      al.push(val);
      top = al.top();
      len = al.len();
    } else {
      val = 2;
      al.push1(val);
      top = al.top();
      len = al.len();
    }
    flag = !flag;
  }

  klee_assert((len == 0) || (((flag == 0) ? (top == 1) : true) &&
                             ((flag == 1) ? (top == 2) : true)));
  return 0;
}
