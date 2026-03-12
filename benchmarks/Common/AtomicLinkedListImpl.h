#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <stdexcept> // For std::out_of_range

#include <algorithm>
#include <functional>

using namespace std;

#define MIN -10
#define MAX 10

class AtomicLinkedList {
public:
    std::vector<int> all;

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

constexpr uint8_t CMD_INSERTHEAD = 0x01;
constexpr uint8_t CMD_POPHEAD = 0x02;

constexpr int DATA_SIZE = 1000;

int data[DATA_SIZE];
int data_index = 0;

void prepareData(int min, int max) {
  srand(42);
    
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

void init (AtomicLinkedList &all, unsigned char *buf, size_t buf_size){
  data_index = 0;

  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_INSERTHEAD: {
      int k = getValue();
      all.insertHead(k);
      break;
    }

    case CMD_POPHEAD: {
      int ret1 = all.popHead();
      break;
    }

    default:
      break;
    }
  }
}
