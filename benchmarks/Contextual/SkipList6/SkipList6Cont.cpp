#include "../../Impl/SkipListImpl.h"

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

    rc::check("testing abstract program", [&ceFile](SkipList sl) {
        int N = *rc::gen::inRange(50, 100);
        
        int i = 0;
        while (i < N) {
            int v = 1;
            
            bool isPresent = sl.isPresent(v);
            int min = sl.min();
            int max = sl.max();
            int len = sl.len();

            sl.insert(v);

            bool isPresent1 = sl.isPresent(v);
            int min1 = sl.min();
            int max1 = sl.max();
            int len1 = sl.len();

            bool expr_insert = (((max <= (- 12)) and (isPresent <= 0)) or (isPresent > 0));

            if (!expr_insert) {
                ceFile << "(insert v=" << v
		       << ", isPresent=" << isPresent
                       << ", min=" << min
		       << ", max=" << max
		       << ", len=" << len
                       << ", isPresent1=" << isPresent1
		       << ", min1=" << min1
                       << ", max1=" << max1
		       << ", len1=" << len1
		       << ")\n";
            }
            RC_ASSERT(expr_insert);
            i++;
        }

        int k = 1;
        int len = sl.len();

        bool ret1 = sl.remove(k);

        int len1 = sl.len();

        bool expr_remove = (true);

        if (!expr_remove) {
            ceFile << "(remove k=" << k
		   << ", len=" << len 
                   << ", len1=" << len1
		   << ", ret1=" << ret1
		   << ")\n";
        }
        RC_ASSERT(expr_remove);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
