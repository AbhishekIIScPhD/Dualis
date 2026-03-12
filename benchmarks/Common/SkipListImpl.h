#include <set>
#include <iostream>
#include <limits.h>
#include <filesystem>
#include <fstream>

#define MIN -10
#define MAX 10

// using set from C++ STL
// set and skiplist have same operations
// set is an associative container implemented using red-black tree.
// associative container stores (key, value) pairs
class SkipList {
 private:
  std::set<int> sl;

 public:
  SkipList(const std::vector<int>& initial_data) {
    sl.insert(initial_data.begin(), initial_data.end());
  }

  bool insert(int data) {
    return sl.insert(data).second;
  }

  int isPresent(int data){
    auto search = sl.find(data);
    return  search != sl.end() ? 1 : 0;
  }

  int remove(int data) {
    auto it = sl.find(data);
    if (it != sl.end()) {
      int removed_value = *it;
      sl.erase(it);
      return removed_value;
    }
    return MIN; // Return MIN if not found
  }

  int upper_bound(int data) {
    auto it = sl.upper_bound(data);
    if (it != sl.end()) {
      return *it;
    }
    return MAX; // Return MAX if no upper bound exists
  }

  int lower_bound(int data) {
    auto it = sl.lower_bound(data);
    if (it != sl.end()) {
      return *it;
    }
    return MIN; // Return MIN if no lower bound exists
  }
  
  int min() const {
    if (sl.empty()) {
      return MAX; // Return sentinel value for empty list
    }
    return *sl.begin();
  }

  int max() const {
    if (sl.empty()) {
      return MIN; // Return sentinel value for empty list
    }
    return *sl.rbegin();
  }

  void print() const {
    std::cout << "List: ";
    for (int val : sl) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }

  int len() const {
    return sl.size();
  }
    
  int contains(int data) const {
    if(sl.empty()){
      std::cout << "set is empty";
    }
    return sl.count(data) > 0 ? 1 : 0;
  }
    
  const std::set<int>& get_data_set() const {
    return sl;
  }

};


constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_REMOVE = 0x02;

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

void init (SkipList &skl, unsigned char *buf, size_t buf_size){
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
      skl.insert(v);
      break;
    }

    case CMD_REMOVE: {
      int v = getValue();
      skl.remove(v);
      break;
    }
    default:
      break;
    }
  }
}
