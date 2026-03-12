#include <vector>
#include <limits.h> // for INT_MAX
#include <algorithm>

#include "seahorn/seahorn.h"

#define MIN -10
#define MAX 11

using namespace std;

class Min {
private:
  vector<short> ml;  // Vector to hold the queue elements

public:
  Min() = default;

  Min(vector<short> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(short e) {
    ml.push_back(e);  
  }

  int len()  {
    return ml.size();
  }

  int minElem()  {
    if (ml.empty()) {
      return MAX;
    }
    auto result = *min_element(ml.cbegin(), ml.cend());
    return result;
  }
};
