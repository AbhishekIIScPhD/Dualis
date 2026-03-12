#include <vector>
#include <limits.h> // for INT_MAX
#include <algorithm>

#include "seahorn/seahorn.h"

#define MIN -11
#define MAX 10

using namespace std;

class Max {
private:
  vector<short> ml;  // Vector to hold the queue elements

public:
  Max() = default;

  Max(vector<short> elements) {
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


  int maxElem()  {
    if (ml.empty()) {
      return MIN;
    }
    auto result = *max_element(ml.cbegin(), ml.cend());
    return result;
  }

};
