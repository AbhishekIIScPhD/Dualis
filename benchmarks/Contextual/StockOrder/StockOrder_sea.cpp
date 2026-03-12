#include "../../SeaImpl/StockOrderImpl.h"


#include <stdio.h>

extern int nd();

int main(int argc, char* argv[]) {
    StockOrder so;
    int N = nd(), minDiff = MAX, len = 0;

    __VERIFIER_assume(N > 0);

    // Initialize with N stock-order pairs
    for (int i = 0; i < N; i++) {
        int stock = nd();
        int order = nd();
        __VERIFIER_assume(stock >= 0 && order >= 0 && order <= stock);
        so.addStockOrder(std::make_pair(stock, order));
	minDiff = so.getMinDiff();
	len = so.len();
    }

    sassert(len < 0 or minDiff >= 0);

    return 0;
}
