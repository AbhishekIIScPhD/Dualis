#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <algorithm>

using namespace std;

#define MIN -10
#define MAX 11


class RedBlackTree {
private:
  std::vector<short> tree;  // Vector to hold the tree nodes

public:
  RedBlackTree(std::vector<short> elements) {
    for (auto e : elements) {
      insert(e);
    }
  }
    
  void insert(short key) {
    tree.push_back(key);  // Insert the key at the end of the vector (tree)
    int currentIndex = tree.size() - 1;

    while (currentIndex > 0) {
      int parentIndex = (currentIndex - 1) / 2;
      if (tree[currentIndex] < tree[parentIndex]) {

	std::swap(tree[currentIndex], tree[parentIndex]);
	currentIndex = parentIndex;
      } else {
	break;
      }
    }
  }

  // Search for a key in the rbt
  bool search(short key) const {
    for (int i = 0; i < tree.size(); i++) {
      if (tree[i] == key) {
	return true;  // Key found
      }
    }
    return false;  // Key not found
  }

  int noDup(){
    set<int> s(tree.begin(), tree.end());
    return s.size() == tree.size();
  }

  int min() const {
    if (tree.empty()) {
      std::cerr << "Tree is empty!" << std::endl;
      return MAX;
    }
    auto result = std::min_element(tree.begin(), tree.end());
    return *result;
  }

  int max() const {
    if (tree.empty()) {
      std::cerr << "Tree is empty!" << std::endl;
      return MIN-1;
    }
    auto result = std::max_element(tree.begin(), tree.end());
    return *result;
  }

  int len() const {
    return tree.size();
  }

  bool isEmpty() const { return tree.size() == 0; }
};

constexpr uint8_t CMD_PUSH = 0x01;

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

void init (RedBlackTree &rbt, unsigned char *buf, size_t buf_size){
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
      rbt.insert(v);
      break;
    }

    case CMD_SEARCH: {
      int v = getValue();
      rbt.search(v);
      break;
    }

    default:
      break;
    }
  }
}
