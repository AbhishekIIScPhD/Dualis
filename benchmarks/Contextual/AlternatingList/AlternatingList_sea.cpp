#include "../../SeaImpl/AlternatingListImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  AlternatingList al;
  int N = nd();

  __VERIFIER_assume(N >= 0);

  // Initialize the list with some elements
  for (int i = 0; i < N; i++) {
    int v = nd();
    if (i % 2 == 0) {
      al.push(v);
    } else {
      al.push1(v);
    }
  }

  int flag = 1;
  int loop_count = nd();
  int top = 0;
  int len = 0;
  __VERIFIER_assume(loop_count >= 0);

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

  sassert((len == 0) || (((flag == 0) ? (top == 1) : true) && ((flag == 1) ? (top == 2) : true)));
  return 0;
}
