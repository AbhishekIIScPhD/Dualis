#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>
#include <filesystem>

using namespace std;

#define MIN -10
#define MAX 10

class BinaryTree {
private:
  std::vector<short> tree;

public:
  BinaryTree(std::vector< short> elements) {
    for (auto e : elements) {
      insert(e);
    }
  }
  
  void insert(int key) {
    tree.push_back(key);  
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

  int search(int key) const {
    for (int i = 0; i < tree.size(); i++) {
      if (tree[i] == key) {
	return 1;  
      }
    }
    return 0;  
  }

  int min() const {
    if (tree.empty()) {
      std::cerr << "Tree is empty!" << std::endl;
      return MAX;
    }
    return tree[0];
  }

  int isEmpty() const { return tree.size() == 0; }

};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_SEARCH = 0x02;

constexpr int MIN_VAL = -1000;
constexpr int MAX_VAL = 1000;
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

void init(BinaryTree &bt, unsigned char *buf, size_t buf_size){
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
	bt.insert(v);
	break;
      }

      case CMD_SEARCH: {
	int v = getValue();
	bt.search(v);
	break;
      }

      default:
	break;
      }
    }
}
