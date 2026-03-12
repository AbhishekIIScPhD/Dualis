#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>       // The header for std::multiset
#include <utility>   // For std::pair

// Define reasonable ranges for generated test values
#define MIN -10
#define MAX 100

// Teach RapidCheck how to generate random std::multiset<int> objects.
namespace rc {
  template <>
  struct Arbitrary<std::multiset<int>> {
    static Gen<std::multiset<int>> arbitrary() {
      return gen::map(gen::container<std::vector<int>>(gen::inRange(MIN+1, MAX)),
		      [](const std::vector<int>& vec){
			return std::multiset<int>(vec.begin(), vec.end());
		      });
    }
  };
}


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << std::endl;
    return 1;
  }
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing abstract program", [&ceFile](std::multiset<int> set) {
    int N = *rc::gen::inRange(1, MAX);
        
    int i = 0;
    while (i < N) {
      int v = 1;
            
      int countvo = set.count(1);
      int countvt = set.count(2);
      int len = set.size();

      set.emplace(v);

      int countvo1 = set.count(1);
      int countvt1 = set.count(2);
      int len1 = set.size();

      bool expr_emplace = (true);

      if (!expr_emplace) {
	ceFile << "(emplace v=" << v
	       << ", countvo=" << countvo
	       << ", countvt=" << countvt
	       << ", len=" << len
	       << ", countvo1=" << countvo1 
	       << ", countvt1=" << countvt1
	       << ", len1=" << len1
	       << ")\n";
      }
      RC_ASSERT(expr_emplace);
      i++;
    }

    int v = 2;
    int countvo = set.count(1);
    int countvt = set.count(v);
    int len = set.size();

    auto emplace1 = [&set](int v){
      set.emplace(v);
    };

    emplace1(v);

    int countvo1 = set.count(1);
    int countvt1 = set.count(v);
    int len1 = set.size();
        
    bool expr_emplace1 = (true);

    if (!expr_emplace1) {
      ceFile << "(emplace1 v=" << v
	     << ", countvo=" << countvo
	     << ", countvt=" << countvt
	     << ", len=" << len
	     << ", countvo1=" << countvo1 
	     << ", countvt1=" << countvt1
	     << ", len1=" << len1
	     << ")\n";
    }
    RC_ASSERT(expr_emplace1);
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
