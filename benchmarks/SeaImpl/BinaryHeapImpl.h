#include <vector>
#include <limits.h> // for INT_MAX

#include <algorithm>
#include <functional>
#include "seahorn/seahorn.h"

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
