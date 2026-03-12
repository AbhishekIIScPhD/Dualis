#include "../../Impl/FlatHashSetImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

    rc::check("testing FlatHashSet Insert", [&ceFile](FlatHashSet fhs) {
        int v = *rc::gen::inRange(MIN, MAX);
        int len = fhs.len();
        int min = fhs.minKey();
        int max = fhs.maxKey();
	int containsv = fhs.contains(v);
        fhs.insert(v);
        int len1 = fhs.len();
        int min1 = fhs.minKey();
        int max1 = fhs.maxKey();
	int containsv1 = fhs.contains(v);
        
	bool expr = (false);

        if (!expr) {
            ceFile << "(insert v=" << v
		   << ", len=" << len
		   << ", containsv=" << containsv
                   << ", min=" << min
		   << ", max=" << max
		   << ", len1=" << len1
		   << ", containsv1=" << containsv1
                   << ", min1=" << min1
		   << ", max1=" << max1
		   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
