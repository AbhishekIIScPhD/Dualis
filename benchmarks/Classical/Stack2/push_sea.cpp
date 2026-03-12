#include "../../SeaImpl/StackImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  Stack st;
  int c = 0, d = 0, N, v;

  __VERIFIER_assume( N >= 0);

  for (int i = 0; i < N; i++){
    v = nd();
    st.push(v);
  }

  int sl = st.len();
  st.push(N);
  int sl1 = st.len();
  bool expr_push = (((sl1 - sl) <= 1) && ((sl1 - sl) > 0));
  sassert(expr_push);
}
