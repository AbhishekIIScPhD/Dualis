#include <vector>
#include <map>       // The header for std::multimap
#include <utility>   // For std::pair

#include "seahorn/seahorn.h"

extern int nd();

int main(int argc, char* argv[]) {
    std::multimap<int, int> map;
    int N = nd(), len = 0, countko = 0;

    __VERIFIER_assume(N > 0);

    for (int i = 0; i < N; i++) {
        int k = 1;
        int v = 1;
	printf("k %d, v %d\n", k, v);
        map.emplace(k, v);
    }

    int k = 1;
    countko = map.count(k);
    len = map.size();

    printf("countko %d, len %d \n", countko, len);
    sassert(countko == N);

    return 0;
}
