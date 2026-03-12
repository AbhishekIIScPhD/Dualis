#include "../../SeaImpl/FlatHashMapImpl.h"

#undef MAX
#define MAX 300

extern int nd();

int main(int argc, char* argv[]) {
    FlatHashMap fhm;
    int N, remove_count, len, containsk;
    
    __VERIFIER_assume(N >= 0);

    int i = 0;
    while (i < N) {
        int k = i;
        int v = i;
        
        fhm.insert(k, v);

        len = fhm.len();
        containsk = fhm.contains(k);

        i++;
    }

    remove_count = fhm.len();
    fhm.remove_all();
    len = fhm.len();

    sassert((remove_count == N) and ( len == 0));

    return 0;
}
