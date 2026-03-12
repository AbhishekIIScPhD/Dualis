#include "../../SeaImpl/FlatHashMapImpl.h"


extern int nd();

int main(int argc, char* argv[]) {
    FlatHashMap fhm;
    int N=nd(), len, containsk;
    int flag = 1;
    int ret = 0;
    
    __VERIFIER_assume(N > 0);

    int i = 0;
    while (i < N) {
        int k = i;
        int v = i;
        
        fhm.insert(k, v);
        len = fhm.len();
        containsk = fhm.contains(k);
        
        i++;
    }

    i = 0;
    flag = 1;
    ret = 0;

    while(i < N) {
        int k = i;
        ret = fhm.erase(k, flag);
        len = fhm.len();
	i++;
        flag = 1 - flag; 
    }

    sassert(ret == (N - 1));
    return 0;
}
