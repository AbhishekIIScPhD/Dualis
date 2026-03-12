#include "../../Impl/AtomicLinkedListImpl.h"

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

    rc::check("testing abstract program", [&ceFile](AtomicLinkedList all) {
        int N = *rc::gen::inRange(1, 20);
        
        int k = 0;
        while (k < N) {
            int len = all.len();
            
            all.insertHead(k);
            
            int len1 = all.len();

            bool expr_insertHead = (true);

            if (!expr_insertHead) {
                ceFile << "(insertHead k=" << k
		       << ", len=" << len
                       << ", len1=" << len1
		       << ")\n";
            }
            RC_ASSERT(expr_insertHead);
            k++;
        }

        int items_processed = 0;
        int len = all.len();
        while (len > 0) {
            int ret1 = all.popHead();
            int len1 = all.len();

            bool expr_popHead = (true);
            
            if (!expr_popHead) {
                ceFile << "(popHead len=" << len
		       << ", ret1=" << ret1 
                       << ", len1=" << len1
		       << ")\n";
            }
            RC_ASSERT(expr_popHead);
            
            items_processed++;
            len = len1;
        }
        
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
