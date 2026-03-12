#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max

#define MIN -10
#define MAX 100 // since the container is a set and map takes only
		// unique key entries we have a higher max value.


//std::map is an associative container that contains
//key-value pairs with unique keys. Search, insertion, and removal of
//elements have average constant-time complexity.

// inspired form AtomicHashMap from Folly, emplace/insert does not
// rewrite on collision, returns pointer to the exiting (key, value)
// pair, hence using emplace instead of insert in from map.
class HashTable{
private:
  std::map<int, int> ht;
  
public:
  void insert (int key, int value){
    ht.emplace(key, value);
  }

  void insert1 (int key, int value){
    ht.emplace(key, value);
  }

  int find (int key){
    auto search = ht.find(key);
    return  search != ht.end()? search->second: MAX; // SHRT_MAX means not found
  }

  int minKey(){
    if(ht.empty()){
      return MAX;
    }
    auto min_key = std::min_element(ht.begin(), ht.end(), [](const auto &a, const auto &b){ // first , second
      return a.first < b.first; // assume first is smallest, and if
				// second (new element) is smaller
				// than smallest (first) then update
    });
    return min_key->first;
  }

  int maxKey(){
    if(ht.empty()){
      return MIN;
    }
    auto max_key = std::max_element(ht.begin(), ht.end(), [](const auto &a, const auto &b){// first, second
      return a.first < b.first; // assume first is largest, and if
				// second (new element) is larger than
				// largest (first) then update
    });
    return max_key->first;    
  }

  int len(){
    return ht.size();
  }

  int contains(int k){
    return ht.contains(k);
  }
};


constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_FIND = 0x02;

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

void init (AtomicHashMap &ahm, unsigned char *buf, size_t buf_size){
  data_index = 0;

  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_INSERT: {
      int k = getValue();
      int v = k;
      ahm.insert(k, v);
      break;
    }

    case CMD_FIND: {
      int k = getValue();
      int ret1 = ahm.find(k);
      break;
    }

    default:
      break;
    }
  }
}
