#include "../../SeaImpl/StackImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  Stack st;
  int c = 0, d = 0, N, v;

  __VERIFIER_assume( N >= 0);

  for (int i = 0; i < N; i++){
    v = nd();
    st.push(v);
  }

  int sl = st.len();
  st.pop();
  int sl1 = st.len();
  bool expr = (((sl - sl1) > 0) and ((sl - sl1) <= 1));
  sassert(expr);
  return 0;
}
