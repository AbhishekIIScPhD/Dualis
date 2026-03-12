#include <set>
#include <limits.h>
#include <vector>

#include "seahorn/seahorn.h"

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
  SkipList() = default;

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

  // void print() const {
  //   for (int val : sl) {
  //     std::cout << val << " ";
  //   }
  // }

  int len() const {
    return sl.size();
  }
    
  int contains(int data) const {
    // if(sl.empty()){
    //   std::cout << "set is empty";
    // }
    return sl.count(data) > 0 ? 1 : 0;
  }
    
  const std::set<int>& get_data_set() const {
    return sl;
  }

};
