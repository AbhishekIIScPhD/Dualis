#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>

#define MIN -10
#define MAX 10

using namespace std;

class DLLCircular {
private:
  std::vector<short> dlc;

public:
  DLLCircular(std::vector<short> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
    
  void push(int key) {
    dlc.push_back(key);
  }

  int max() const {
    if (dlc.empty()){
      cerr << "DLL_Circular is empty\n";
      return MIN;
    }
    auto result = *max_element(dlc.cbegin(), dlc.cend());
    return result;
  }

  int len() const {
    return dlc.size();
  }
};

constexpr uint8_t CMD_PUSH = 0x01;

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

void init (DLLCircular &dlc, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_PUSH: {
      int v = getValue();
      dlc.push(v);
      break;
    }
    default:
      break;
    }
  }
}
