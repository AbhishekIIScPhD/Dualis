#include "../../SeaImpl/LruCacheImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  LRUCache lruc;
  int N = nd(), len = 0, lru = MAX, containslru = 0, containsk = 0;

  __VERIFIER_assume(N >= 3);

  int i = 0;
  while (i < N) {
    int k = i;
    int v = i;
    
 
    lruc.insert_or_assign(k, v);

    len = lruc.len();
    lru = lruc.getLru();
    containslru = lruc.contains(lru);

    i++;
  }

  int k = nd();
  __VERIFIER_assume(k >= 0 && k < N);
  
  containsk = lruc.contains(k);
  lruc.find(k);
  lru = lruc.getLru();

  sassert(lru == k);

  return 0;
}
