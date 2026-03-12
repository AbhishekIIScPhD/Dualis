#include <list>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>

#define MIN -10
#define MAX 10

class LRUCache {
 private:
  int capacity;
  std::list<std::pair<int, int>> cache_list;
  std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache_map;

 public:
 LRUCache(int capacity) : capacity(capacity) {
    if (capacity < 1) {
      this->capacity = 1;
    }
  }

  int getCapacity(){
    return capacity;
  }

  int len(){
    return cache_list.size();
  }
  
  void insert_or_assign(int key, int value) {
    auto it = cache_map.find(key);
    if (it != cache_map.end()) {
      it->second->second = value;
      cache_list.splice(cache_list.begin(), cache_list, it->second); // moving the it->value to the beginning of the list in cache_list
      return;
    }
    if (cache_map.size() == capacity) {
      int key_to_evict = cache_list.back().first;
      cache_map.erase(key_to_evict);
      cache_list.pop_back();
    }
    cache_list.push_front({key, value});
    cache_map[key] = cache_list.begin();
  }

  int find(int key) {
    auto it = cache_map.find(key);
    if (it == cache_map.end()) {
      return MAX; // returning sentinel values
    }
    cache_list.splice(cache_list.begin(), cache_list, it->second);
    return it->second->second;
  }

  int contains(int key) {// does not change the order, only get/put
			 // does.
    return cache_map.count(key) > 0 ? 1 : 0;
  }

  int getMru() {
    if (cache_list.empty()) {
      return MAX; // returning sentinel value
    }
    return cache_list.front().second;
  }

  int getLru() {
    if (cache_list.empty()) {
      return MAX; // returning sentinel value
    }
    return cache_list.back().second;
  }
};

#include <tuple> // Required for std::tuple

constexpr uint8_t CMD_INSERTORASSIGN = 0x01;
constexpr uint8_t CMD_FIND = 0x02;
constexpr uint8_t CMD_GETLRU = 0x03;

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

void init (LRUCache &lru, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_INSERT_OR_ASSIGN: {
      int v = getValue();
      int k = v;
      lru.insert_or_assign(k, v);
      break;
    }
    case CMD_FIND: {
      int v = getValue();
      lru.find(v);
      break;
    }
    case CMD_GETLRU: {
      int lru = lru.getLRU();
      break;
    }
    default:
      break;
    }
  }
}
