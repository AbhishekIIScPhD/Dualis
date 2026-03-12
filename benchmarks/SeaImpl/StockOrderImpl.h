#include <vector>
#include <limits.h> // for INT_MAX
#include <cmath>

#include "seahorn/seahorn.h"

using namespace std;

#define MIN -10
#define MAX 10

class StockOrder {
 private:
  vector<pair<short, short>> cal;
  int md;

 public:
  StockOrder() = default;

  StockOrder(vector<pair<short, short>> elements) {
    md = 0;
    for (auto e : elements) {
      addStockOrder(e);
    }
  }
  
  void addStockOrder(pair<short, short> evp) {
    if((abs(evp.second - evp.first)) <= md){
      md = abs(evp.second - evp.first);
    }
    cal.push_back(evp);
  }

  int len() const {
    return cal.size();
  }

  int getMinDiff(){
    return md;
  }
};
