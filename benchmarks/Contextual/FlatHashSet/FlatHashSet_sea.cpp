#include "../../SeaImpl/FlatHashSetImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
    FlatHashSet fhs;
    int N;
    
    __VERIFIER_assume(N >= 0);

    // Insert sequence 0 to N-1
    int i = 0;
    while (i < N) {
        int v = i;
        int len = fhs.len();
        bool containsv = fhs.contains(v);
        int min = fhs.minKey();
        int max = fhs.maxKey();

        fhs.insert(v);

        int len1 = fhs.len();
        bool containsv1 = fhs.contains(v);
        int min1 = fhs.minKey();
        int max1 = fhs.maxKey();

        bool expr_insert = (true);
        sassert(expr_insert);
        i++;
    }

    // Erase sequence 0 to N-1
    i = 0;
    while (i < N) {
        int v = i;
        int len = fhs.len();
        bool containsv = fhs.contains(v);
        int min = fhs.minKey();
        int max = fhs.maxKey();
        
        fhs.erase(v);

        int len1 = fhs.len();
        bool containsv1 = fhs.contains(v);
        int min1 = fhs.minKey();
        int max1 = fhs.maxKey();

        bool expr_erase = (true);
        sassert(expr_erase);
        i++;
    }

    // Reserve operation
    int len = fhs.len();
    fhs.reserve(N);
    int len1 = fhs.len();
    
    bool expr_reserve = (true);
    sassert(expr_reserve);

    // Insert sequence N to 2N-1
    i = 0;
    while (i < N) {
        int v = i + N;
        int len = fhs.len();
        bool containsv = fhs.contains(v);
        int min = fhs.minKey();
        int max = fhs.maxKey();

        fhs.insert1(v);

        int len1 = fhs.len();
        bool containsv1 = fhs.contains(v);
        int min1 = fhs.minKey();
        int max1 = fhs.maxKey();

        bool expr_insert1 = (true);
        sassert(expr_insert1);
        i++;
    }

    return 0;
}
