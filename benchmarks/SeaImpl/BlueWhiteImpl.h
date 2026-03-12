#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#include "seahorn/seahorn.h"


#define MIN -10
#define MAX 10

using namespace std;

class BWList {
private:
  std::vector<short> bwl;

public:
  BWList() = default;

  BWList(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  // 0 - blue, 1 - white
  void push(int key) {
    bwl.push_back(key);
  }

  // 0 - blue, 1 - white
  void push1(int key) {
    bwl.push_back(key);
  }

  int blue_present(){
    find(bwl.begin(), bwl.end(), 0);
    return 0;
  }

  int blue_count(){
    return count(bwl.begin(), bwl.end(), 0);
  }

  int top() const{
    return bwl.at(0);
  }

  int len() const{
    return bwl.size();
  }
};
