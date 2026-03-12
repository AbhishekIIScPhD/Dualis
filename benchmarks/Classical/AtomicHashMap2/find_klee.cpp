#include "../../KleeImpl/AtomicHashMapImpl.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << "\n";
    return 1;
  }
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::out);

  prepareData(MIN, MAX);
 HashTable table) {
    int k = *rc::gen::inRange(MIN+1, MAX); 
    // RC_PRE(table.contains(k) == 1);

    int len = table.len();
    int min = table.minKey();
    int max = table.maxKey();
    int containsk = table.contains(k);
    int ret1 = table.find(k);
      
    bool expr = (k <= (- 98));

    if (!expr) {
      ceFile << "(find k=" << k
	     << ", len=" << len
	     << ", min=" << min
	     << ", max=" << max
	     << ", containsk=" << containsk
	     << ", ret1=" << ret1
	     << ")\n" ;
    }
    klee_assert(expr);
  

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << "\n";
  return 0;
}
