#include <set>
#include <stdio.h>

#include "seahorn/seahorn.h"

extern int nd();

int main(int argc, char* argv[])
{
  std::multiset<int> set;
  int N = nd(), v = nd(), len = 0, countv = 0;

  __VERIFIER_assume(N > 0);

  for (int i = 0; i < N; i++) {
    v = nd();
    set.emplace(v);
    printf("v %d \n", v);
  }

  len = set.size();
  countv = set.count(v);
  printf("len %d countv  %d \n", len, countv);

  sassert(len == N and countv == N);

  return 0;
}
