#include "../../SeaImpl/StockOrderImpl.h"

extern int nd();

int main(int argc, char* argv[])
{
  StockOrder obj;
  int N;

  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; i++) {
    int stock = nd();
    int order = nd();
    auto p = make_pair(stock, order);
    obj.addStockOrder(p);
  }

  int len = obj.len();
  int minDiff = obj.getMinDiff();
  int stock = nd();
  int order = nd();
  auto p = make_pair(stock, order);
  obj.addStockOrder(p);
  int len1 = obj.len();
  int minDiff1 = obj.getMinDiff();
  bool expr = ((stock - minDiff) > (-100));

  sassert(expr);

  return 0;
}
