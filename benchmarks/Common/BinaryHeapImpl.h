#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <stdexcept> // For std::out_of_range

#include <algorithm>
#include <functional>

#define MIN -5
#define MAX 5

using namespace std;

class BinaryHeap {
public:
  std::vector<int> heap;

  BinaryHeap() = default;
  
  BinaryHeap(std::vector<int> elements) {
    heap = elements;
    std::make_heap(heap.begin(), heap.end(), std::greater<int>());
  }

  void insert(int key) {
    heap.push_back(key);
    std::push_heap(heap.begin(), heap.end(), std::greater<int>());
  }

  int deleteMin() {
    if (heap.empty()) {
      return MAX; // undefined behavior
    }
    int min_element = heap.front();
    std::pop_heap(heap.begin(), heap.end(), std::greater<int>());
    heap.pop_back();
    return min_element;
  }

  void downHeap() {
    size_t size = heap.size();
    if (size <= 1) {
      return;
    }

    size_t parent = 0;
    size_t child;

    while ((child = 2 * parent + 2) < size) {
      if (heap[child] < heap[child - 1]) {
      } else {
	child = child - 1;
      }

      if (heap[parent] > heap[child]) {
	std::iter_swap(heap.begin() + parent, heap.begin() + child);
	parent = child;
      } else {
	return;
      }
    }

    child = 2 * parent + 1;
    if (child < size && heap[parent] > heap[child]) {
      std::iter_swap(heap.begin() + parent, heap.begin() + child);
    }
  }
    
  int len() const {
    return heap.size();
  }

  bool isEmpty() const {
    return heap.empty();
  }

  bool isHeap() const {
    return std::is_heap(heap.begin(), heap.end(), std::greater<int>());
  }

  int min() const {
    if (heap.empty()) {
      return MAX; // undefined behavior
    }
    return heap.front();
  }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_DELETEMIN = 0x02;
constexpr uint8_t CMD_DOWNHEAP = 0x03;

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

void init (BinaryHeap &bh, unsigned char *buf, size_t buf_size){
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
      bh.insert(k);
      break;
    }

    case CMD_DELETEMIN: {
      int ret1 = bh.deleteMin();
      break;
    }

    case CMD_DOWNHEAP: {
      bh.downHeap();
      break;
    }

    default:
      break;
    }
  }
}
