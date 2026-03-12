#include "../../Impl/MinImpl.h"

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

    rc::check("testing abstract program", [&ceFile](Min ml) {
        int lmin = MAX;
        int gmin = MAX;
        int loop_count = *rc::gen::inRange(10, 50);

        for (int i = 0; i < loop_count; ++i) {
            int v = *rc::gen::inRange(MIN, MAX);
            
            lmin = ml.minElem();
            
            ml.append(v);
            
            int lmin1 = ml.minElem();
            
            bool expr_append = (true);

            if (!expr_append) {
                ceFile << "(append v=" << v
		       << ", lmin=" << lmin 
                       << ", lmin1=" << lmin1
		       << ")\n";
            }
            RC_ASSERT(expr_append);

            if (gmin > v) {
                gmin = v;
            }
        }        
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
