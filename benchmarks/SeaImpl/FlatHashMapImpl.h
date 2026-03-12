#include <vector>
#include <set>
#include <unordered_map>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max

#include "seahorn/seahorn.h"

#define MIN -10
#define MAX 100

// flathashmap has the same interface as that unordered_map but is a
// more performant hybrid version.
class FlatHashMap{
private:
  std::unordered_map<int, int> fhm;

public:
  FlatHashMap() = default;

  void insert(int key, int value){
    fhm.emplace(key, value);
  }

  int erase(int key) {
    size_t elements_erased = fhm.erase(key);

    if (elements_erased > 0) {
      return key;
    } else {
      return MIN;
    }
  }

  int erase(int key, int flag) {
    if (flag == 1) {
      if (fhm.erase(key) > 0) {
	return key;
      }
    }
    return MIN;
  }

  int contains(int k){
    return fhm.find(k) != fhm.end();
  }

  void remove_all(){
    fhm.clear();
  }

  int remove_none(){
    return 0;
  }

  
  int len(){
    return fhm.size();
  }

  int minKey(){
    if(fhm.empty()){
      return MIN;
    }
    auto min_key = std::min_element(fhm.begin(), fhm.end(), [](const auto &a, const auto &b){ // first , second
      return a.first < b.first; // assume first is smallest, and if
      // second (new element) is smaller
      // than smallest (first) then update
    });
    return min_key->first;
  }

  int maxKey(){
    if(fhm.empty()){
      return MAX;
    }
    auto max_key = std::max_element(fhm.begin(), fhm.end(), [](const auto &a, const auto &b){// first, second
      return a.first < b.first; // assume first is largest, and if
      // second (new element) is larger than
      // largest (first) then update
    });
    return max_key->first;    
  }
};
