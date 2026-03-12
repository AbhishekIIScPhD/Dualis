#include <vector>
#include <limits.h> // for INT_MAX
#include <stack>
#include "seahorn/seahorn.h"

using namespace std;

#define MIN -10
#define MAX 10

class AlternatingList {
 private:
  std::vector<short> al;

 public:

  AlternatingList() = default;
  
  AlternatingList(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
  
  void push(short key) {
    al.push_back(key);
  }

  // second instance of push
  void push1(short key) {
    al.push_back(key);
  }

  short top() const{
    if (al.size() > 0) // undefined behavior when back() is called on
		       // 0 length vector
      return al.back();
    else
      return MAX;
  }

  short len() const{
    return al.size();
  }
};
