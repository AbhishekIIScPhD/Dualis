#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for SHORT_MAX
#include <filesystem>
#include <algorithm>
#include <utility>

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
  NormalFilterQueue(vector<pair<short, short>> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(pair<short, short> packet) {
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
      cerr << "queue is empty!" << endl;
      return PRIO;
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int minPacketSize()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MINPACK;
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<1>(lhs) > get<1>(rhs);
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
  srand(42); // Fixed seed for consistent fuzzing
    
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i] = min + (rand() % (max - min + 1));
  }
  data_index = 0;
}

void preparePacketSize(int min, int max) {
  srand(42); // Fixed seed for consistent fuzzing
    
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


void init (NormalFilterQueue &nfq, unsigned char *buf, size_t buf_size){
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
      
      nfq.append(v);
      break;
    }
    case CMD_PROCESSQUEUE: {
      nfq.processQueue();
      break;
    }
    default:
      break;
    }
  }
}


