#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>       // The header for std::multiset
#include <utility>   // For std::pair

#define MIN -10
#define MAX 10

// building a structure by defining an arbitrary function returning a
// generate multiset and we define the generator of a set of multisets
// with the set of values to populate with.
namespace rc {
  template <>
  struct Arbitrary<std::multiset<int>> {
    static Gen<std::multiset<int>> arbitrary() {
      return gen::map(gen::container<std::vector<int>>(gen::inRange(MIN, MAX)),
		      [](const std::vector<int>& vec) {
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
        int N = *rc::gen::inRange(1, 20);
        
        int v = *rc::gen::inRange(SHRT_MIN, SHRT_MAX);
        int i = 0;
        int len = 0;
        int countv1 = 0;

        while (i < N) {
            len = set.size();
            int countv = set.count(v);

            set.emplace(v);

            countv1 = set.count(v);
            int len1 = set.size();

            bool expr_emplace = (true);

            if (!expr_emplace) {
                ceFile << "(emplace v=" << v
		       << ", countv=" << countv
		       << ", len=" << len
                       << ", countv1=" << countv1
		       << ", len1=" << len1
		       << ")\n";
            }
            RC_ASSERT(expr_emplace);
            i++;
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
