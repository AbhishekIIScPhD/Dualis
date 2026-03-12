#include "../../SeaImpl/AtomicHashMapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  HashTable ht;
  int N = nd();
  int len, min, max, containsk;
  
  __VERIFIER_assume(N >= 0);

  // First insert loop
  for (int i = 0; i < N; i++) {
    int k = i;
    int v = i;
    
    ht.insert(k, v);

    len = ht.len();
    min = ht.minKey();
    max = ht.maxKey();
    containsk = ht.contains(k);
  }

  // Second insert loop
  for (int i = 0; i < N; i++) {
    int k = i;
    int v = i;

    ht.insert1(k, v);

    len = ht.len();
    min = ht.minKey();
    max = ht.maxKey();
    containsk = ht.contains(k);

  }

  int ret = ht.find(k);

  sassert(ret == min);
  return 0;
}
