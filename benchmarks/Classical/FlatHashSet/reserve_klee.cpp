#include "../../KleeImpl/FlatHashSetImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

  prepareData(MIN, MAX);
FlatHashSet fhs) {
        int N = *rc::gen::inRange(MIN+1, MAX);
        int len = fhs.len();
        fhs.reserve(N);
        int len1 = fhs.len();
        
	bool expr = len1 == len;

        if (!expr) {
            ceFile << "(reserve len=" << len
                   << ", N=" << N
                   << ", len1=" << len1
		   << ")\n";
        }
        klee_assert(expr);
    

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
