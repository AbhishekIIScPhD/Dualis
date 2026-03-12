#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#define MIN -10
#define MAX 10

using namespace std;

class BWList {
private:
  std::vector<short> bwl;

public:
  BWList(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  // 0 - blue, 1 - white
  void push(int key) {
    bwl.push_back(key);
  }

    // 0 - blue, 1 - white
  void push1(int key) {
    bwl.push_back(key);
  }

  int blue_present(){
    find(bwl.begin(), bwl.end(), 0);
    return 0;
  }

  int blue_count(){
    return count(bwl.begin(), bwl.end(), 0);
  }

  int top() const{
    return bwl.at(0);
  }

  int len() const{
    return bwl.size();
  }
};

constexpr uint8_t CMD_PUSH = 0x01;

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

void init (BlueWhite &bw, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_PUSH: {
      int value_to_push = getValue();
      bw.push(value_to_push);
      break;
    }
    default:
      break;
    }
  }
}
