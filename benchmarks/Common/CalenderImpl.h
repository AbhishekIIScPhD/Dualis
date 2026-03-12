#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#define MIN 0
#define MAX 10

using namespace std;

class Cal {
private:
  vector<pair<short, short>> cal;
  int md;
  bool md_set = false;

public:
  Cal(vector<pair<short, short>> elements) {
    md = 0;
    for (auto e : elements) {
      insert(e);
    }
  }
  
  void insert(pair<int, int> evp) {
    if(not md_set){
      md = abs(evp.first - evp.second);
      md_set = true;
    }else if(abs(evp.first - evp.second) >= md){
      md = abs(evp.first - evp.second);
    }
    cal.push_back(evp);
  }

  int len() const {
    return cal.size();
  }

  int maxDiff(){
    return md;
  }
};

constexpr uint8_t CMD_INSERT = 0x01;

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

void init (Calender &cal, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_INSERT: {
      int v1 = getValue();
      int v2 = getValue();
      auto p =  make_pair(ev1, ev2);
      cal.insert(p);
      break;
    }
    default:
      break;
    }
  }
}
