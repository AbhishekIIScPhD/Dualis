#include <vector>
#include <string>
#include <algorithm>
#include <climits>

#include "seahorn/seahorn.h"

#define MIN 0
#define MAX 10

using namespace std;

class Cal {
private:
  vector<pair<short, short>> cal;
  int md;
  bool md_set = false;

public:
  Cal() = default;

  Cal(vector<pair<short, short>> elements) {
    md = 0;
    for (auto e : elements) {
      insert(e);
    }
  }
  
  void insert(pair<int, int> evp) {
    if(not md_set){
      md = abs(evp.first - evp.second);
      md_set = true;
    }else if(abs(evp.first - evp.second) >= md){
      md = abs(evp.first - evp.second);
    }
    cal.push_back(evp);
  }

  int len() const {
    return cal.size();
  }

  int maxDiff(){
    return md;
  }
};
