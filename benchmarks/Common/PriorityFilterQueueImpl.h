#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <algorithm>

#define MAXPACK 499
#define MINPACK 1
#define PRIO 1

using namespace std;

class PriorityFilterQueue {
private:
  vector<pair<short, short>> Q;  // Vector to hold the queue elements

public:
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
      cerr << "queue is empty!" << endl;
      return PRIO; 
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int maxPacketSize()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MAXPACK;  // Return maximum possible value for empty tree
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<1>(lhs) < get<1>(rhs);
    });
    return get<1>(result);
  }

  bool isEmpty() { return Q.size() == 0; }
};

constexpr uint8_t CMD_APPEND = 0x01;
constexpr uint8_t CMD_PROCESSQUEUE = 0x02;

constexpr int DATA_SIZE = 1000;

int prio[DATA_SIZE];
int ps[DATA_SIZE];
int data_index = 0;

void preparePriority(int min, int max) {
  srand(42);
    
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i] = min + (rand() % (max - min + 1));
  }
  data_index = 0;
}

void preparePacketSize(int min, int max) {
  srand(42);
    
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i] = min + (rand() % (max - min + 1));
  }
  data_index = 0;
}

int getPacketSize() {
  int value = ps[data_index];
  data_index = (data_index + 1) % DATA_SIZE;
  return value;
}

int getPriority() {
  int value = pr[data_index];
  data_index = (data_index + 1) % DATA_SIZE;
  return value;
}


void init (PriorityFilterQueue &pfq, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_APPEND: {
      int v = getValue();
      pfq.append(v);
      break;
    }
    case CMD_PROCESSQUEUE: {
      pfq.processQueue();
      break;
    }
    default:
      break;
    }
  }
}
