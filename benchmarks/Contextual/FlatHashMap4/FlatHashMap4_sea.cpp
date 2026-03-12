#include "../../SeaImpl/FlatHashMapImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    FlatHashMap fhm;
    int N = nd(), len = 0, ret, contains_k = 0;
    
    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; i++) {
        int k = i;
        int v = i;
        
        fhm.insert(k, v);
        len = fhm.len();
        containsk = fhm.contains(k);
    }


    int flag = 0;
    for (int i = 0; i < N; i++) {
      int k = i;
        ret = fhm.erase(k, flag);
        len = fhm.len();
        flag = 1 - flag;
    }

    sassert(ret == N-2);
    return 0;
}
