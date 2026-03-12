#include "../../Impl/AtomicHashMapImpl.h"

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

  rc::check("testing abstract program", [&ceFile](HashTable ht) {
    int N = *rc::gen::inRange(1, SHRT_MAX);
        
    int i = 0;
    while (i < N) {
      int k = i;
      int v = i;
            
      int len = ht.len();
      int min = ht.minKey();
      int max = ht.maxKey();
      bool containsk = ht.contains(k);

      ht.insert(k, v);

      int len1 = ht.len();
      int min1 = ht.minKey();
      int max1 = ht.maxKey();
      bool containsk1 = ht.contains(k);

      bool expr_insert = (true);

      if (!expr_insert) {
	ceFile << "(insert k=" << k
	       << ", v=" << v
	       << ", len=" << len
	       << ", min=" << min
	       << ", max=" << max
	       << ", containsk=" << containsk
	       << ", len1=" << len1
	       << ", min1=" << min1
	       << ", max1=" << max1
	       << ", containsk1=" << containsk1
	       << ")\n";
      }
      RC_ASSERT(expr_insert);
      i++;
    }

    i = 0;
    while (i < N) {
      int k = i;
      int v = i;

      int len = ht.len();
      int min = ht.minKey();
      int max = ht.maxKey();
      bool containsk = ht.contains(k);

      ht.insert1(k, v);

      int len1 = ht.len();
      int min1 = ht.minKey();
      int max1 = ht.maxKey();
      bool containsk1 = ht.contains(k);

      bool expr_insert1 = (true);

      if (!expr_insert1) {
	ceFile << "(insert1 k=" << k
	       << ", v=" << v
	       << ", len=" << len
	       << ", min=" << min
	       << ", max=" << max
	       << ", containsk=" << containsk
	       << ", len1=" << len1
	       << ", min1=" << min1
	       << ", max1=" << max1
	       << ", containsk1=" << containsk1
	       << ")\n";
      }
      RC_ASSERT(expr_insert1);
      i++;
    }

    int min = ht.minKey();
    int k = min;
        
    int max = ht.maxKey();
    int len = ht.len();
    bool containsk = ht.contains(k);
        
    int ret1 = ht.find(k);

    bool expr_find = (true);
        
    if (!expr_find) {
      ceFile << "(find k=" << k
	     << ", len=" << len
	     << ", min=" << min
	     << ", max=" << max
	     << ", containsk=" << containsk
	     << ", ret1=" << ret1 << ")\n";
    }
    RC_ASSERT(expr_find);
        
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
