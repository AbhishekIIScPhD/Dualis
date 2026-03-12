#include <vector>
#include <limits.h> // for INT_MAX
#include <algorithm>
#include <set>

#include "seahorn/seahorn.h"

using namespace std;

#define MIN -10
#define MAX 11


class RedBlackTree {
 private:
  std::vector<short> tree;  // Vector to hold the tree nodes

 public:
  RedBlackTree() = default;

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
      return MAX;
    }
    auto result = std::min_element(tree.begin(), tree.end());
    return *result;
  }

  int max() const {
    if (tree.empty()) {
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
