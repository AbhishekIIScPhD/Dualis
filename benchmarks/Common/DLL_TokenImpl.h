#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <stack>

#define MIN -10
#define MAX 10

using namespace std;

class DLLTok {
private:
  std::vector<short> dt;

public:
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
      std::cerr << "list is empty!" << std::endl;
      return MIN;
    }
    auto result = std::max_element(dt.begin(), dt.end());
    return *result;
  }

  int min() const {
    if (dt.empty()) {
      std::cerr << "List is empty!" << std::endl;
      return MAX;
    }
    auto result = std::min_element(dt.begin(), dt.end());
    return *result;
  }

  int len() const{
    return dt.size();
  }
};

namespace rc {
  template <>
  struct Arbitrary<DLLTok> {
    static Gen<DLLTok> arbitrary() {
      return gen::construct<DLLTok>(gen::container<vector<short>>(gen::arbitrary<short>()));
    }      
  };
}


