#include "../../SeaImpl/AlternatingListImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  AlternatingList al;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int v = nd();
    al.push(v);
  }

  int len = al.len();
  int top = al.top();
  int val = nd();
  al.push(val);
  int len1 = al.len();
  int top1 = al.top();
  bool expr = ((val <= 0) || (((((len1 - len) > (-1)) && ((top1 - val) > (-1)) && ((top1 - val) <= 0) && (val <= 2)) || (val > 2)) && (val > 0)));

  sassert(expr);

  return 0;
}
