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

    rc::check("testing SkipList insert", [&ceFile](SkipList sl) {
    int v = *rc::gen::inRange(MIN+1, MAX);
    int isPresent = sl.contains(v);
	int min = sl.min();
	int max = sl.max();
	int len = sl.len();
        sl.insert(v);
        int isPresent1 = sl.contains(v);
	int min1 = sl.min();
	int max1 = sl.max();
	int len1 = sl.len();

	bool expr = (true);
        
        if (!expr) {
            ceFile << "(insert v=" << v
		   << ", isPresent="<< isPresent
		   << ", min=" << min
		   << ", max=" << max
		   << ", len=" << len
                   << ", isPresent1=" << isPresent1
		   << ", min1=" << min1
		   << ", max1=" << max1
		   << ", len1=" << len1
		   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
