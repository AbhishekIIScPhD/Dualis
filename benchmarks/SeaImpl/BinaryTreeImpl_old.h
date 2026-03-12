#include <vector>
#include <climits>
#include "seahorn/seahorn.h"

using namespace std;

class BinaryTree {
private:
  std::vector<int> tree;

public:
  BinaryTree() = default;

  BinaryTree(std::vector<int> elements) {
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
      return INT_MAX;
    }
    return tree[0]; 
  }

  int isEmpty() const { return tree.size() == 0; }

};
