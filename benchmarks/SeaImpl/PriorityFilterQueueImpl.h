#include <vector>
#include <limits.h> // for INT_MAX
#include <algorithm>

#include "seahorn/seahorn.h"

#define MAXPACK 499
#define MINPACK 1
#define PRIO 1

using namespace std;

class PriorityFilterQueue {
private:
  vector<pair<short, short>> Q;  // Vector to hold the queue elements

public:
  PriorityFilterQueue() = default;

  PriorityFilterQueue(vector<pair<short, short>> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(pair<short, short> packet) {
    Q.push_back(packet);  
  }

  void processQueue() {
    for (auto it = Q.begin(); it != Q.end(); ) {
      if (std::get<0>(*it) == 1 && std::get<1>(*it) < 500) {
	it = Q.erase(it);
      } else {
	++it;
      }
    }
  }
 
  int len()  {
    return Q.size();
  }

  int minPrio()  {
    if (Q.empty()) {
      return PRIO; 
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int maxPacketSize()  {
    if (Q.empty()) {
      return MAXPACK;  // Return maximum possible value for empty tree
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<1>(lhs) < get<1>(rhs);
    });
    return get<1>(result);
  }

  bool isEmpty() { return Q.size() == 0; }
};
