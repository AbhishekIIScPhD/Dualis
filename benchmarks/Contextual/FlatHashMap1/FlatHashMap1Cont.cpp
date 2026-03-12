#include "../../Impl/FlatHashMapImpl.h"

#undef MAX

#define MAX 300

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

        int len = fhm.len();
        int remove_count = 0;

        int remove_count1 = fhm.len();

	fhm.remove_all();
        
        int len1 = fhm.len();

        bool expr_remove_all = (true);
        
        if (!expr_remove_all) {
            ceFile << "(remove_all len=" << len
		   << ", remove_count=" << remove_count
                   << ", len1=" << len1
		   << ", remove_count1=" << remove_count1 << ")\n";
        }
        RC_ASSERT(expr_remove_all);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
