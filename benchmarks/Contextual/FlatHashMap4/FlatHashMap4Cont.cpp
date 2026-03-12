#include "../../Impl/FlatHashMapImpl.h"

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

    rc::check("testing abstract program", [&ceFile](FlatHashMap fhm) {
        int N = *rc::gen::inRange(1, MAX);
        
        int i = 0;
        while (i < N) {
            int k = i;
            int v = i;
            
            int len = fhm.len();
            bool containsk = fhm.contains(k);

            fhm.insert(k, v);

            int len1 = fhm.len();
            bool containsk1 = fhm.contains(k);

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert k=" << k
		       << ", v=" << v
		       << ", len=" << len
                       << ", containsk=" << containsk
		       << ", len1=" << len1
                       << ", containsk1=" << containsk1
		       << ")\n";
            }
            RC_ASSERT(expr_insert);
            i++;
        }

        i = 0;
        int flag = 0;
        int ret1 = 0;

        while(i < N) {
            int k = i;
            int len = fhm.len();

            ret1 = fhm.erase(k, flag);
            
            int len1 = fhm.len();

            bool expr_erase = (true);
            
            if (!expr_erase) {
                ceFile << "(erase k=" << k << " len=" << len << " flag=" << flag
                       << " len1=" << len1 << " ret1=" << ret1 << ")\n";
            }
            RC_ASSERT(expr_erase);
            
            i++;
            flag = 1 - flag; 
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
