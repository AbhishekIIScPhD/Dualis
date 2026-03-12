#include "../../Impl/AtomicHashMapImpl.h"

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

    rc::check("testing abstract program", [&ceFile](HashTable ahm) {
        int N = *rc::gen::inRange(1, MAX);
        
        int i = 0;
        while (i < N) {
            int k = i;
            int v = i;
            
            int len = ahm.len();
            int min = ahm.minKey();
            int max = ahm.maxKey();
            bool containsk = ahm.contains(k);

            ahm.insert(k, v);

            int len1 = ahm.len();
            int min1 = ahm.minKey();
            int max1 = ahm.maxKey();
            bool containsk1 = ahm.contains(k);

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert k=" << k
		       << ", v=" << v
		       << ", len=" << len
                       << ", min=" << min
		       << ", max=" << max
		       << ", containsk=" << containsk
                       << ", len1=" << len1
		       << ", min1=" << min1
		       << ", max1=" << max1
                       << ", containsk1=" << containsk1
		       << ")\n";
            }
            RC_ASSERT(expr_insert);

            v = i + 1;

            len = ahm.len();
            min = ahm.minKey();
            max = ahm.maxKey();
            containsk = ahm.contains(k);

            ahm.insert1(k, v);

            len1 = ahm.len();
            min1 = ahm.minKey();
            max1 = ahm.maxKey();
            containsk1 = ahm.contains(k);

            bool expr_insert1 = (true);

            if (!expr_insert1) {
	      ceFile << "(insert1 k=" << k
		     << ", v=" << v
		     << ", len=" << len
		     << ", min=" << min
		     << ", max=" << max
		     << ", containsk=" << containsk
		     << ", len1=" << len1
		     << ", min1=" << min1
		     << ", max1=" << max1
		     << ", containsk1=" << containsk1
		     << ")\n";
            }
            RC_ASSERT(expr_insert1);

            i++;
        }

        int k = *rc::gen::inRange(0, N);
        
        int min = ahm.minKey();
        int max = ahm.maxKey();
        int len = ahm.len();
        bool containsk = ahm.contains(k);
        
        int ret1 = ahm.find(k);

        bool expr_find = (((k - ret1) <= 0) and ((ret1 - k) <= 0));
        
        if (!expr_find) {
	  ceFile << "(find k=" << k
		 << ", len=" << len  
		 << ", min=" << min 
		 << ", max=" << max
		 << ", containsk=" << containsk 
		 << ", ret1=" << ret1 << ")\n";
        }
        RC_ASSERT(expr_find);
        
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
