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
        int N = *rc::gen::inRange(1, 20);
        
        int i = 0;
        while (i < N) {
            int v = *rc::gen::inRange(SHRT_MIN, SHRT_MAX);

            bool isPresent = sl.isPresent(v);
            int min = sl.min();
            int max = sl.max();
            int len = sl.len();

            sl.insert(v);

            bool isPresent1 = sl.isPresent(v);
            int min1 = sl.min();
            int max1 = sl.max();
            int len1 = sl.len();

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert v=" << v
		       << ", isPresent=" << isPresent
                       << ", min=" << min
		       << ", max=" << max
		       << ", len=" << len
                       << ", isPresent1=" << isPresent1
		       << ", min1=" << min1
                       << ", max1=" << max1
		       << ", len1=" << len1 << ")\n";
            }
            RC_ASSERT(expr_insert);
            i++;
        }

        int max = sl.max();
        int lb_ret1 = sl.lower_bound(max);

        bool expr_lower_bound = (true);

        if (!expr_lower_bound) {
            ceFile << "(lower_bound max=" << max
		   << ", lb_ret1=" << lb_ret1 << ")\n";
        }
        RC_ASSERT(expr_lower_bound);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
