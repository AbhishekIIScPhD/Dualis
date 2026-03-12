#include "../../Impl/LruCacheImpl.h"

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

  rc::check("testing abstract program", [&ceFile](LRUCache lruc) {
    // here N is controlling both values inserted and length of the
    // datastructure, hence not bounding it by MAX.
    
    int N = *rc::gen::inRange(3, MAX);
    int size = N;
        
    int i = 0;
    while (i < N) {
      int k = i;
      int v = i;
            
      int len = lruc.len();
      int lru = lruc.getLru();
      bool containslru = lruc.contains(lru);

      int ret1 = 0;
      lruc.insert_or_assign(k, v);

      int len1 = lruc.len();
      int lru1 = lruc.getLru();
      bool containslru1 = lruc.contains(lru1);

      bool expr_insert_or_assign = (true);

      if (!expr_insert_or_assign) {
	ceFile << "(insert_or_assign k=" << k
	       << ", v=" << v
	       << ", len=" << len
	       << ", lru=" << lru
	       << ", size=" << size
	       << ", containslru=" << containslru
	       << ", len1=" << len1
	       << ", lru1=" << lru1 
	       << ", containslru1=" << containslru1
	       << ", ret1=" << ret1 << ")\n";
      }
      RC_ASSERT(expr_insert_or_assign);
      i++;
    }

    int k = *rc::gen::inRange(0, N);
    bool containsk = lruc.contains(k);

    lruc.find(k);
        
    int lru1 = lruc.getLru();

    bool expr_find = (false);
        
    if (!expr_find) {
      ceFile << "(find k=" << k
	     << ", containsk=" << containsk
	     << ", lru1=" << lru1 << ")\n";
    }
    RC_ASSERT(expr_find);
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
