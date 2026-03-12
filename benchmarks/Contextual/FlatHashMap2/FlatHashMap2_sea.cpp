#include "../../SeaImpl/FlatHashMapImpl.h"

extern int nd();

#undef MAX
#define MAX 1000

int main(int argc, char* argv[]) {
    FlatHashMap fhm;
    int N = nd(), len = 0, containsk = 0, remove_count = 0;

    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; i++) {
        int k = i;
        int v = i;
        
        fhm.insert(k, v);

        len = fhm.len();
        containsk = fhm.contains(k);
    }

    remove_count = fhm.remove_none();
    len = fhm.len();
    sassert(len == N);
    return 0;
}
