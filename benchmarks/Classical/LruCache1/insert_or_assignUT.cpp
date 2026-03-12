#include "../../Impl/LruCacheImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

    rc::check("testing LruCache Insert_or_assign", [&ceFile](LRUCache lc) {
        int k = *rc::gen::inRange(MIN, MAX);
        int v = k;
	int N = *rc::gen::inRange(0, MAX);
        int len = lc.len();
        int lru = lc.getLru();
        int size = lc.getCapacity();
        int containslru = lc.contains(lru);
        lc.insert_or_assign(k, v);
	int ret1 = 0;
        int len1 = lc.len();
        int lru1 = lc.getLru();
        int containslru1 = lc.contains(lru1);
        
	bool expr = (false);

        if (!expr) {
            ceFile << "(insert_or_assign k=" << k
                   << ", v=" << v
                   << ", len=" << len
                   << ", lru=" << lru
                   << ", size=" << size
                   << ", containslru=" << containslru
                   << ", len1=" << len1
                   << ", lru1=" << lru1
                   << ", containslru1=" << containslru1
                   << ", ret1=" << ret1
                   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
