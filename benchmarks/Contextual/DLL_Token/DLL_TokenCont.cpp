#include "../../Impl/DLL_TokenImpl.h"

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

    rc::check("testing abstract program", [&ceFile](DLLTok dt) {
        int loop_count = *rc::gen::inRange(0, 20);

        for (int i = 0; i < loop_count; ++i) {
            int val = 0;
            
            int min = dt.min();
            int len = dt.len();
            int max = dt.max();
            
            dt.push(val);
            
            int min1 = dt.min();
            int max1 = dt.max();
            int len1 = dt.len();

            bool expr_push = (true);

            if (!expr_push) {
                ceFile << "(push val=" << val 
                       << ", min=" << min 
                       << ", len=" << len
                       << ", max=" << max 
                       << ", min1=" << min1
                       << ", len1=" << len1 
                       << ", max1=" << max1
                       << ")\n";
            }
            RC_ASSERT(expr_push);
        }


    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
