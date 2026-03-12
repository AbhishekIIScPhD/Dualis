#include <vector>
#include <limits.h> // for SHORT_MAX
#include <algorithm>
#include <utility>
#include <stdio.h>

#include "seahorn/seahorn.h"

#define MAX 511
#define MIN 0
#define PRIO 0

#define MAXPACK 511
#define MINPACK 500

using namespace std;

class NormalFilterQueue {
private:
  vector<pair<short, short>> Q;  // Vector to hold the queue elements

public:
  NormalFilterQueue() = default;

  NormalFilterQueue(vector<pair<short, short>> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(pair<short, short> packet) {
    printf("In append method\n");
    Q.push_back(packet);  
  }

  void processQueue() {
    for (int i=0; i < Q.size(); i++){
      if(!Q.empty()){
	if(get<0>(Q[i]) == 1 and get<1>(Q[i]) < 500){
	  Q.erase(Q.begin()+i);
	}
      }
    }
  }

  int len()  {
    return Q.size();
  }

  int maxPrio()  {
    if (Q.empty()) {
        return PRIO;
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int minPacketSize()  {
    if (Q.empty()) {
        return MINPACK;
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<1>(lhs) > get<1>(rhs);
    });
    return get<1>(result);
  }

  bool isEmpty() { return Q.size() == 0; }
};
