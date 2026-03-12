#include <vector>
#include <stdio.h>
#include <map>
#include <set>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max
// #include "seahorn/seahorn.h"

#define MIN -10
#define MAX 100 // since the container is a set and map takes only
		// unique key entries we have a higher max value.


//std::map is an associative container that contains
//key-value pairs with unique keys. Search, insertion, and removal of
//elements have average constant-time complexity.

// inspired form AtomicHashMap from Folly, emplace/insert does not
// rewrite on collision, returns pointer to the exiting (key, value)
// pair, hence using emplace instead of insert in from map.
class HashTable{
private:
  std::map<int, int> ht;
  
public:
  void insert (int key, int value){
    printf("insert :: key : %d, value = %d\n", key , value);
    ht.emplace(key, value);
  }

  void insert1 (int key, int value){
    printf("insert :: key : %d, value = %d\n", key , value);
    // std::cout << "Inseret : key :" << key <<"value :" << value << "\n"; 
    ht.emplace(key, value);
  }

  int find (int key){
    printf("Map contents:\n");
    for (const auto& pair : ht) {
        printf("Key: %d, Value: %d\n", pair.first, pair.second);
    }
    printf("Total elements: %zu\n", ht.size());

    printf("find :: key : %d len : %d \n", key, this->len());
    // std::cout << "Find : key :" << key << "\n"; 
    auto search = ht.find(key);
    printf("value : %d\n", search->second);
    if (search != ht.end()){
      printf("search not equal to end\n");
    }else{
      printf("search equal to end\n");
    }
    return  search != ht.end()? search->second: MAX; // SHRT_MAX means not found
  }

  int minKey(){
    if(ht.empty()){
      return MAX;
    }
    auto min_key = std::min_element(ht.begin(), ht.end(), [](const auto &a, const auto &b){ // first , second
      return a.first < b.first; // assume first is smallest, and if
				// second (new element) is smaller
				// than smallest (first) then update
    });
    return min_key->first;
  }

  int maxKey(){
    if(ht.empty()){
      return MIN;
    }
    auto max_key = std::max_element(ht.begin(), ht.end(), [](const auto &a, const auto &b){// first, second
      return a.first < b.first; // assume first is largest, and if
				// second (new element) is larger than
				// largest (first) then update
    });
    return max_key->first;    
  }

  int len(){
    return ht.size();
  }

  int contains(int k){
    return ht.find(k) != ht.end();
  }
};
