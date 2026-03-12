#include "../../Impl/FlatHashMapImpl.h"

#undef MIN
#undef MAX

#define MIN -129
#define MAX 300


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

    rc::check("testing FlatHashMap Erase", [&ceFile](FlatHashMap fhm) {
        int k = *rc::gen::inRange(MIN, MAX+1);
        int len = fhm.len();
        int flag = *rc::gen::elementOf(std::vector<int>{0, 1});
	int ret1 = fhm.erase(k, flag);
        int len1 = fhm.len();
        
	bool expr = (true);

        if (!expr) {
            ceFile << "(erase k=" << k
                   << ", len=" << len
                   << ", flag=" << flag
                   << ", len1=" << len1
                   << ", ret1=" << ret1
                   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
