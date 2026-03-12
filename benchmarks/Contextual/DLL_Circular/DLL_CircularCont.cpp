#include "../../Impl/DLL_CircularImpl.h"

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

    rc::check("testing abstract program", [&ceFile](DLLCircular dlc) {
        int val = 0;
        int N = *rc::gen::inRange(10, 50);

        for (int i = 0; i < N; ++i) {
            bool condition = *rc::gen::inRange(0, 2);
            if (condition) {
                if (val < 3) {
                    val = val + 1;
                }
            }

            int max = dlc.max();
            dlc.push(val);
            int max1 = dlc.max();

            bool expr_push = (true);

            if (!expr_push) {
                ceFile << "(push val=" << val
		       << ", max=" << max 
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
