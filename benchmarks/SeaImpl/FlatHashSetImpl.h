#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max

#define SMAX 101
#define SMIN -11

#define MIN -10
#define MAX 100

// flathashset has the same interface as that unordered_set but is a
// more performant hybrid version.
class FlatHashSet{
 private:
  std::unordered_set<int> fhs;

 public:
  void insert(int value){
    fhs.emplace(value);
  }

  void insert1(int value){
    fhs.emplace(value);
  }

  void erase(int key){
    fhs.erase(key);
  }

  int contains(int k){
    return fhs.contains(k);
  }

  void reserve(int size){
    fhs.reserve(size);
  }

  int len(){
    return fhs.size();
  }

  int minKey(){
    if(fhs.empty()){
      return SMAX;
    }
    auto min_key = std::min_element(fhs.begin(), fhs.end(), [](const auto &a, const auto &b){ // first , second
	return a < b; // assume first is smallest, and if
	// second (new element) is smaller
	// than smallest (first) then update
      });
    return *min_key;
  }

  int maxKey(){
    if(fhs.empty()){
      return SMIN;
    }
    auto max_key = std::max_element(fhs.begin(), fhs.end(), [](const auto &a, const auto &b){// first, second
	return a < b; // assume first is largest, and if
	// second (new element) is larger than
	// largest (first) then update
      });
    return *max_key;    
  }
};


namespace rc {
  template<>
    struct Arbitrary<FlatHashSet> {
    static Gen<FlatHashSet> arbitrary() {
      return gen::map(gen::container<std::set<int>>(gen::inRange(MIN, MAX)),
		      [](const std::set<int>&values) {
			FlatHashSet fhs;
			for (int value : values){
			  fhs.insert(value);
			}
			return fhs;
		      });
    }
  };
}

