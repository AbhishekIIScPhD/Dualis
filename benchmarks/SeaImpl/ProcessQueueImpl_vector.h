#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#include "seahorn/seahorn.h"

#define MIN -10
#define MAX 10

class ProcessQueue {
 private:
  std::vector<int> Q;  // Vector to hold the queue elements

 public:
  ProcessQueue() = default;
  
  ProcessQueue(std::vector<int> elements) {
    for (auto e : elements) {
      insert(e);
    }
  }

  void insert(int nexttime) {
    Q.push_back(nexttime);  
  }

  int len()  {
    return Q.size();
  }

  int minttw()  {
    if (Q.empty()) {
      return MAX;
    }
    return *(std::min_element(Q.begin(), Q.end()));
  }

  void choosenext() {
    for (auto it = Q.begin(); it != Q.end();) {
      if (*it == 1) {
	it = Q.erase(it);
      } else {
	(*it)--;
	++it;
      }
    }
  }

  bool isEmpty() { return Q.size() == 0; }
};
