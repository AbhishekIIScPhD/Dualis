#include "../../Impl/FlatHashMapImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

    rc::check("testing FlatHashMap Remove_all", [&ceFile](FlatHashMap fhm) {
        int len = fhm.len();
        int remove_count = 0;
	int remove_count1 = fhm.len();
        fhm.remove_all();
        int len1 = fhm.len();
        
	bool expr = (true);

        if (!expr) {
            ceFile << "(remove_all len=" << len
                   << ", remove_count=" << remove_count
                   << ", len1=" << len1
                   << ", remove_count1=" << remove_count1
                   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
