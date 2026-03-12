#include <vector>
#include <limits.h> // for INT_MAX
#include <algorithm>
#include <functional>

#include "seahorn/seahorn.h"

using namespace std;

#define MIN -10
#define MAX 10

class AtomicLinkedList {
public:
    std::vector<int> all;

  AtomicLinkedList() = default;

    AtomicLinkedList(std::vector<int> elements) {
        all = elements;
    }

    void insertHead(int key) {
        all.insert(all.begin(), key);
    }

    int popHead() {
        if (all.empty()) {
            return MAX; 
        }
        int head_val = all.front();
        all.erase(all.begin());
        return head_val;
    }

    int getHead() const {
        if (all.empty()) {
            return MAX;
        }
        return all.front();
    }

    int len() const {
        return all.size();
    }

    int max(){
    if(all.empty()){
      return MIN;
    }
    int max_val = *std::max_element(all.begin(), all.end());
    return max_val;
  }
};
