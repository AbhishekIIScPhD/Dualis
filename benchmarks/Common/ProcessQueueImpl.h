#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#define MIN -10
#define MAX 10

class ProcessQueue {
 private:
  std::vector<int> Q;  // Vector to hold the queue elements

 public:
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
      std::cerr << "queue is empty!" << std::endl;
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

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_CHOOSENEXT = 0x02;

constexpr int DATA_SIZE = 1000;

int data[DATA_SIZE];
int data_index = 0;

void prepareData(int min, int max) {
  srand(42); // Fixed seed for consistent fuzzing
    
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i] = min + (rand() % (max - min + 1));
  }
  data_index = 0;
}

int getValue() {
  int value = data[data_index];
  data_index = (data_index + 1) % DATA_SIZE;
  return value;
}

void init (ProcessQueue &pq, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_INSERT: {
      int v = getValue();
      pq.insert(v);
      break;
    }

    case CMD_CHOOSENEXT: {
      pq.choosenext();
      break;
    }

    default:
      break;
    }
  }
}
