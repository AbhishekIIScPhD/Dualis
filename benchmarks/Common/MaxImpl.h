#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <algorithm>

#define MIN -11
#define MAX 10

using namespace std;

class Max {
private:
  vector<short> ml;  // Vector to hold the queue elements

public:
  Max(vector<short> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(short e) {
    ml.push_back(e);  
  }

  int len()  {
    return ml.size();
  }


  int maxElem()  {
    if (ml.empty()) {
      cerr << "list is empty!" << endl;
      return MIN;
    }
    auto result = *max_element(ml.cbegin(), ml.cend());
    return result;
  }

};

constexpr uint8_t CMD_APPEND = 0x01;

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

void init (Max &maxl, unsigned char *buf, size_t buf_size){
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
      maxl.append(v);
      break;
    }
    default:
      break;
    }
  }
}
