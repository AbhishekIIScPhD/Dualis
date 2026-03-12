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
    
    rc::check("testing SkipList remove", [&ceFile](SkipList sl) {
        int len = sl.len();
	int k = *rc::gen::inRange(MIN+1, MAX);
        int ret1 = sl.remove(k);// returns (1/0) if data (is removed/did not exist)
        int len1 = sl.len();
	bool expr = (true);
        
        if (!expr) {
	  ceFile << "(remove k =" << k
		 << ", len =" << len
		 << ", len1 =" << len1
		 << ", ret1 =" << ret1
		 << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    return 0;
}
