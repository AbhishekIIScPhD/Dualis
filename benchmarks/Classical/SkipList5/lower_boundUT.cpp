#include "../../Impl/SkipListImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::cout << "Final Path: " << filePath << std::endl;
    std::ofstream ceFile(filePath, std::ios::out);
    if (!ceFile) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return 1;
    }

    rc::check("testing SkipList lower_bound", [&ceFile](SkipList sl) {
      int min = sl.min();
        
        int lb_ret1 = sl.lower_bound(min);
	bool expr = (k <= (- 1));
        
        if (!expr) {
            ceFile << "(lower_bound min=" << min
		   << ", lb_ret1=" << lb_ret1
		   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
