#include "../../SeaImpl/StackImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  int c = 0, d = 0, v;
  int N;

  Stack st;
  __VERIFIER_assume(N > 0);
        
  while (c < N) {
    v = nd();
    st.push(N);
    c = c + 1;
  }

  while (st.len() != 0) {
    st.pop();
    d = d + 1;
  }


  sassert(c == d);
  return 0;
}
