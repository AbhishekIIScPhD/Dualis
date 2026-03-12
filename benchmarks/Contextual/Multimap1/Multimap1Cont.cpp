#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>       // The header for std::multimap
#include <utility>   // For std::pair

#define MIN -10
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
    if (!ceFile.is_open()) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return 1;
    }

    rc::check("testing abstract program", [&ceFile](std::multimap<int, int> map) {

        int N = *rc::gen::inRange(1, 20);
        
        int i = 0;
        while (i < N) {
            int k = 1;
            int v = *rc::gen::arbitrary<int>();
            
            int countko = map.count(k);
            int len = map.size();

            map.emplace(k, v);

            int countko1 = map.count(k);
            int len1 = map.size();

            bool expr_emplace = (true);

            if (!expr_emplace) {
                ceFile << "(emplace k=" << k
		       << ", v=" << v
		       << ", countko=" << countko
                       << ", len=" << len
		       << ", countko1=" << countko1 
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
