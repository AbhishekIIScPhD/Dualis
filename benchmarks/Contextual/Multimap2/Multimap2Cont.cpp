#include <rapidcheck.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>       // The header for std::multimap
#include <utility>   // For std::pair

#define MIN -10
#define MAX 200

namespace rc {
    template <>
    struct Arbitrary<std::multimap<int, int>> {
        static Gen<std::multimap<int, int>> arbitrary() {
             return gen::map(
                 gen::container<std::vector<std::pair<int, int>>>(
                    gen::pair(gen::inRange(MIN+1, MAX), gen::inRange(MIN+1, MAX))
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
            int v = *rc::gen::inRange(MIN+1, MAX);
            
            int countko = map.count(k);
            int countkt = map.count(2);
            int len = map.size();

            map.emplace(k, v);

            int countko1 = map.count(k);
            int countkt1 = map.count(2);
            int len1 = map.size();

            bool expr_emplace = (true);

            if (!expr_emplace) {
                ceFile << "(emplace k=" << k
		       << ", v=" << v
		       << ", countko=" << countko
                       << ", countkt=" << countkt
		       << ", len=" << len
                       << ", countko1=" << countko1
		       << ", countkt1=" << countkt1
                       << ", len1=" << len1 << ")\n";
            }
            RC_ASSERT(expr_emplace);
            i++;
        }

        int v = *rc::gen::inRange(MIN+1, MAX);
        int k = 2;

        int countko = map.count(1);
        int countkt = map.count(k);
        int len = map.size();

	auto emplace1 = [&map](int k , int v){
	  map.emplace(k, v);
	};

	emplace1(k , v);

        int countko1 = map.count(1);
        int countkt1 = map.count(k);
        int len1 = map.size();

        bool expr_emplace1 = (true);

        if (!expr_emplace1) {
            ceFile << "(emplace1 k=" << k
		   << ", v=" << v
		   << ", countko=" << countko
                   << ", countkt=" << countkt
		   << ", len=" << len
                   << ", countko1=" << countko1
                   << ", countkt1=" << countkt1
		   << ", len1=" << len1
		   << ")\n";
        }
        RC_ASSERT(expr_emplace1);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
