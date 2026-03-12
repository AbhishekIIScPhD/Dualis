#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max

#define MIN -10
#define MAX 300

// flathashmap has the same interface as that unordered_map but is a
// more performant hybrid version.
class FlatHashMap{
private:
  std::unordered_map<int, int> fhm;

public:
  void insert(int key, int value){
    fhm.emplace(key, value);
  }

  int erase(int key) {
    size_t elements_erased = fhm.erase(key);

    if (elements_erased > 0) {
      return key;
    } else {
      return MIN;
    }
  }

  int erase(int key, int flag) {
    if (flag == 1) {
      if (fhm.erase(key) > 0) {
	return key;
      }
    }
    return MIN;
  }

  int contains(int k){
    return fhm.contains(k);
  }

  void remove_all(){
    fhm.clear();
  }

  int remove_none(){
    return 0;
  }

  
  int len(){
    return fhm.size();
  }

  int minKey(){
    if(fhm.empty()){
      return MIN;
    }
    auto min_key = std::min_element(fhm.begin(), fhm.end(), [](const auto &a, const auto &b){ // first , second
      return a.first < b.first; // assume first is smallest, and if
      // second (new element) is smaller
      // than smallest (first) then update
    });
    return min_key->first;
  }

  int maxKey(){
    if(fhm.empty()){
      return MAX;
    }
    auto max_key = std::max_element(fhm.begin(), fhm.end(), [](const auto &a, const auto &b){// first, second
      return a.first < b.first; // assume first is largest, and if
      // second (new element) is larger than
      // largest (first) then update
    });
    return max_key->first;    
  }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_ERASE = 0x02;
constexpr uint8_t CMD_REMOVEALL = 0x03;
constexpr uint8_t CMD_REMOVENONE = 0x04;

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

void init (FlatHashMap &fhm, unsigned char *buf, size_t buf_size){
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
      fhm.push(v);
      break;
    }
    case CMD_ERASE: {
      int flag = getValue() % 2;
      int v = getValue();
      fhm.erase(v, flag);
      break;
    }
    case CMD_REMOEVALL: {
      fhm.remove_all();
      break;
    }
    case CMD_REMOVENONE: {
      fhm.remove_none();
      break;
    }
    default:
      break;
    }
  }
}
