#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>       // The header for std::multimap
#include <utility>   // For std::pair

#define MIN -129
#define MAX 10

namespace rc {
    template <>
    struct Arbitrary<std::multimap<int, int>> {
        static Gen<std::multimap<int, int>> arbitrary() {
             return gen::map(
                 gen::container<std::vector<std::pair<int, int>>>(
                    gen::pair(gen::inRange(MIN, MAX), gen::arbitrary<int>())
                ),
                 [](const std::vector<std::pair<int, int>>& vec) {
                     return std::multimap<int, int>(vec.begin(), vec.end());
                }
            );
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

    rc::check("testing std::multimap emplace", [&ceFile](std::multimap<int, int> map) {
        int k = *rc::gen::inRange(MIN, MAX);
        int v = k;
	int len = map.size();
	int k_one = 1;
	int k_two = 2;
	int countko = map.count(k_one);
	int countkt = map.count(k_two);
        map.emplace(k, v);
	int countko1 = map.count(k_one);
	int countkt1 = map.count(k_two);
	int len1 = map.size();
	bool expr = (k <= 1);
        if (!expr) {
            ceFile << "(emplace k=" << k
		   << ", v=" << v
                   << ", countko=" << countko
		   << ", countkt=" << countkt
		   << ", len=" << len
                   << ", countko1=" << countko1
		   << ", countkt1=" << countkt1
		   << ", len1=" << len1
		   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
