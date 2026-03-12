#include <vector>
#include <string>
#include "seahorn/seahorn.h"

#define MIN -10
#define MAX 11

class Stack {
private:
  std::vector<int> stack;

public:
  Stack() = default;

  Stack(const std::vector<int>& elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  void push(int key) {
    stack.push_back(key);
   }

  void pop() {
     stack.pop_back();
  }

  int len() const {
    return stack.size();
  }
};
