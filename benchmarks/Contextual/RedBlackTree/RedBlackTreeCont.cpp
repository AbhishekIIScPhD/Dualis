#include "../../Impl/RedBlackTreeImpl.h"

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

    rc::check("testing abstract program", [&ceFile](RedBlackTree rbt) {
        int N = *rc::gen::inRange(1, MAX);
        
        int i = 0;
        while (i < N) {
            int max = rbt.max();
            int min = rbt.min();
            int len = rbt.len();
            bool noDup = rbt.noDup();

            rbt.insert(i);

            int max1 = rbt.max();
            int min1 = rbt.min();
            int len1 = rbt.len();
            bool noDup1 = rbt.noDup();

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert i=" << i 
                       << ", max=" << max 
                       << ", max1=" << max1 
                       << ", min=" << min
                       << ", min1=" << min1
                       << ", len=" << len 
                       << ", len1=" << len1 
                       << ", noDup=" << noDup
                       << ", noDup1=" << noDup1 << ")\n";
            }
            RC_ASSERT(expr_insert);
            i = i + 1;
        }

        int data = *rc::gen::inRange(0, N);
        
        int min = rbt.min();
        int max = rbt.max();
        int len = rbt.len();
	int noDup = rbt.noDup();

        bool ret1 = rbt.search(data);

        bool expr_search = (true);
        
        if (!expr_search) {
             ceFile << "(search data=" << data 
                    << ", max=" << max             
                    << ", min=" << min 
                    << ", len=" << len
                    << ", noDup=" << noDup 
                    << ", ret1=" << ret1 << ")\n";
        }
        RC_ASSERT(expr_search);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
