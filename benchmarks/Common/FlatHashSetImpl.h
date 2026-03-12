#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max

#define SMAX 101
#define SMIN -11

#define MIN -10
#define MAX 100

// flathashset has the same interface as that unordered_set but is a
// more performant hybrid version.
class FlatHashSet{
 private:
  std::unordered_set<int> fhs;

 public:
  void insert(int value){
    fhs.emplace(value);
  }

  void insert1(int value){
    fhs.emplace(value);
  }

  void erase(int key){
    fhs.erase(key);
  }

  int contains(int k){
    return fhs.contains(k);
  }

  void reserve(int size){
    if (size < 0) {
      return;
    }
    fhs.reserve(size);
  }

  int len(){
    return fhs.size();
  }

  int minKey(){
    if(fhs.empty()){
      return SMAX;
    }
    auto min_key = std::min_element(fhs.begin(), fhs.end(), [](const auto &a, const auto &b){ // first , second
	return a < b; // assume first is smallest, and if
	// second (new element) is smaller
	// than smallest (first) then update
      });
    return *min_key;
  }

  int maxKey(){
    if(fhs.empty()){
      return SMIN;
    }
    auto max_key = std::max_element(fhs.begin(), fhs.end(), [](const auto &a, const auto &b){// first, second
	return a < b; // assume first is largest, and if
	// second (new element) is larger than
	// largest (first) then update
      });
    return *max_key;    
  }
};


constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_ERASE = 0x02;
constexpr uint8_t CMD_RESERVE = 0x01;

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

void init (FlatHashSet &fhs, unsigned char *buf, size_t buf_size){
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
      fhs.insert(v);
      break;
    }
    case CMD_ERASE: {
      int v = getValue();
      fhs.erase(v);
      break;
    }
    case CMD_RESERVE: {
      int v = getValue();
      fhs.reserve(v);
      break;
    }
    default:
      break;
    }
  }
}
