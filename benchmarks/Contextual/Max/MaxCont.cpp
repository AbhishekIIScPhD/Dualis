#include "../../Impl/MaxImpl.h"

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

    rc::check("testing abstract program", [&ceFile](Max ml) {

        int lmax = MIN;
        int gmax = MIN;
        int loop_count = *rc::gen::inRange(10, 50);

        for (int i = 0; i < loop_count; ++i) {
            int v = *rc::gen::inRange(MIN+1, MAX+1);
            
            int len = ml.len();
            lmax = ml.maxElem();
            
            ml.append(v);
            
            int len1 = ml.len();
            int lmax1 = ml.maxElem();
            
            bool expr_append = (true);

            if (!expr_append) {
                ceFile << "(append v=" << v
		       << ", lmax=" << lmax
                       << ", lmax1=" << lmax1
		       << ")\n";
            }
            RC_ASSERT(expr_append);

            if (gmax < v) {
                gmax = v;
            }
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
