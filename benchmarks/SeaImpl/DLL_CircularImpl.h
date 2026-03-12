#include <vector>
#include <limits.h> // for INT_MAX

#include "seahorn/seahorn.h"


#define MIN -10
#define MAX 10

using namespace std;

class DLLCircular {
private:
  std::vector<short> dlc;

public:
  DLLCircular() = default;
  
  DLLCircular(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  void push(int key) {
    dlc.push_back(key);
  }

  int max() const {
    if (dlc.empty()){
      cerr << "DLL_Circular is empty\n";
      return MIN;
    }
    auto result = *max_element(dlc.cbegin(), dlc.cend());
    return result;
  }

  int len() const {
    return dlc.size();
  }
};
