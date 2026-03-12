#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <algorithm>

#define MIN -10
#define MAX 11

using namespace std;

class Min {
private:
  vector<short> ml;  // Vector to hold the queue elements

public:
  Min(vector<short> elements) {
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

  int minElem()  {
    if (ml.empty()) {
      cerr << "list is empty!" << endl;
      return MAX;
    }
    auto result = *min_element(ml.cbegin(), ml.cend());
    return result;
  }
};

int getValue() {
  int value = data[data_index];
  data_index = (data_index + 1) % DATA_SIZE;
  return value;
}

void init (Min &minl, unsigned char *buf, size_t buf_size){
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
      minl.append(v);
      break;
    }
    default:
      break;
    }
  }
}
