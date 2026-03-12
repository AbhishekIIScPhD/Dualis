#include <map>       // The header for std::multimap
#include <utility>   // For std::pair
#include <stdio>

#include "seahorn/seahorn.h"

extern int nd();

#define MIN -10
#define MAX 200

int main(int argc, char* argv[]) {
    std::multimap<int, int> map;
    int N = nd(), len = 0, countko = 0, countkt = 0;

    __VERIFIER_assume(N > 0);

    int i = 0;
    while (i < N) {
        int k = 1;
        int v = 1;
        

        map.emplace(k, v);

        countko = map.count(k);
        countkt = map.count(2);
        len = map.size();
	printf("k %d, v %d\n", k, v);
	printf("countko %d, countkt %d len %d \n", countko, countkt, len);
        i++;
    }

    int v = 2;
    int k = 2;


    auto emplace1 = [&map](int k, int v) {
        map.emplace(k, v);
    };


    printf("k %d, v %d\n", k, v);
    emplace1(k, v);

    countko = map.count(1);
    countkt = map.count(k);
    len = map.size();

    printf("countko %d, countkt %d\n", countko, countkt);
    sassert(countko == N and countkt == 1);

    return 0;
}
