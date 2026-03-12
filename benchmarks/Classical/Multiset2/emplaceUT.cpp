#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>       // The header for std::multiset
#include <utility>   // For std::pair

// Define reasonable ranges for generated test values
#define MIN -129
#define MAX 10

// Teach RapidCheck how to generate random std::multiset<int> objects.
namespace rc {
  template <>
  struct Arbitrary<std::multiset<int>> {
    static Gen<std::multiset<int>> arbitrary() {
      return gen::map(gen::container<std::vector<int>>(gen::inRange(MIN, MAX)),
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
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing std::multiset emplace", [&ceFile](std::multiset<int> set) {
    int v = *rc::gen::inRange(MIN, MAX);
    int vo = 1;
    int vt = 2;
    int countvo = set.count(vo);
    int countvt = set.count(vt);
    int len = set.size();
    set.emplace(v);
    int countvo1 = set.count(vo);
    int countvt1 = set.count(vt);      
    int len1 = set.size();
	bool expr = (v <= 1);

    if (!expr) {
      ceFile << "(emplace v=" << v
	     << ", countvo=" << countvo
	     << ", countvt=" << countvt
	     << ", len=" << len
	     << ", countvo1=" << countvo1
	     << ", countvt1=" << countvt1
	     << ", len1=" << len1
	     << ")\n";
    }
    RC_ASSERT(expr);
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
