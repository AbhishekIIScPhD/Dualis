#include <vector>
#include <limits.h> // for INT_MAX
#include <stack>
#include <algorithm>

#include "seahorn/seahorn.h"

#define MIN -10
#define MAX 10

using namespace std;

class DLLTok {
private:
  std::vector<short> dt;

public:
    DLLTok() = default;

  DLLTok(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  void push(int key) {
    dt.push_back(key);
  }

  int max() const {
    if (dt.empty()) {
      return MIN;
    }
    auto result = std::max_element(dt.begin(), dt.end());
    return *result;
  }

  int min() const {
    if (dt.empty()) {
      return MAX;
    }
    auto result = std::min_element(dt.begin(), dt.end());
    return *result;
  }

  int len() const{
    return dt.size();
  }
};

