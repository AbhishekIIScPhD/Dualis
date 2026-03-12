#include "../../Impl/FlatHashMapImpl.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a file path for logging." << std::endl;
        return 1;
    }
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::out);

    rc::check("testing FlatHashMap Insert", [&ceFile](FlatHashMap fhm) {
        int k = *rc::gen::inRange(MIN+1, MAX);
        int v = k;
        int len = fhm.len();
        int containsk = fhm.contains(k);
        fhm.insert(k, v);
        int len1 = fhm.len();
        int containsk1 = fhm.contains(k);
        
	bool expr = ((k <= (- 10)) and (containsk1 > 0) and (len1 > 0));

        if (!expr) {
            ceFile << "(insert k=" << k
                   << ", v=" << v
                   << ", len=" << len
                   << ", containsk=" << containsk
                   << ", len1=" << len1
                   << ", containsk1=" << containsk1
                   << ")\n";
        }
        RC_ASSERT(expr);
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
