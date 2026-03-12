#include "../../Impl/BinaryHeapImpl.h"

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

    rc::check("testing abstract program", [&ceFile](BinaryHeap bh) {
        int N = *rc::gen::inRange(1, SHRT_MAX);
        
        int i = 0;
        while (i < N) {
            int len = bh.len();
            bool isHeap = bh.isHeap();
            
            bh.insert(i);
            
            int len1 = bh.len();
            bool isHeap1 = bh.isHeap();

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert i=" << i
		       << ", isHeap=" << isHeap
		       << ", len=" << len
                       << ", isHeap1=" << isHeap1
		       << ", len1=" << len1 << ")\n";
            }
            RC_ASSERT(expr_insert);
            i++;
        }

        int len = bh.len();
        bool isHeap = bh.isHeap();

        int ret1 = bh.deleteMin();

        int len1 = bh.len();
        bool isHeap1 = bh.isHeap();

        bool expr_deleteMin = (true);
        
        if (!expr_deleteMin) {
            ceFile << "(deleteMin len=" << len
		   << ", isHeap=" << isHeap
                   << ", len1=" << len1
		   << ", ret1=" << ret1
		   << ", isHeap1=" << isHeap1 << ")\n";
        }
        RC_ASSERT(expr_deleteMin);

        bh.downHeap();

        bool isHeap2 = bh.isHeap();

        bool expr_downheap = (isHeap2 > 0);

        if (!expr_downheap) {
            ceFile << "(downheap isHeap2=" << isHeap2 << ")\n";
        }
        RC_ASSERT(expr_downheap);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
