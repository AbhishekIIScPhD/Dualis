#include "../../Impl/BinaryTreeImpl.h"


#undef MIN
#undef MAX

#define MIN -10
#define MAX 1000
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

  rc::check("testing abstract program", [&ceFile](BinaryTree bt) {
    int N = *rc::gen::inRange(10, 100); // need to control non-deterministic loop

    for (int i = 0; i < N; ++i) {
      int n = *rc::gen::inRange(MIN+1, MAX);
      if (n >= 0) {
	int min = bt.min();
	int isEmpty = bt.isEmpty();
	bt.insert(n);
	int min1 = bt.min();
	int isEmpty1 = bt.isEmpty();
                
	bool expr_insert = (((((isEmpty > (- 1)) and ((min1 - min) <= 0)) or ((min1 - min) > 0)) and (min1 <= 0)) or ((isEmpty > (- 1)) and (min1 > 0)));

	if (!expr_insert) {
	  ceFile << "(insert n=" << n
		 << ", min=" << min
		 << ", min1=" << min1
		 << ", isEmpty=" << isEmpty
		 << ", isEmpty1=" << isEmpty1 << ")\n";
	}
	RC_ASSERT(expr_insert);
      }
    }

    int v = *rc::gen::inRange(MIN, 0);
    int min = bt.min();
    bool isEmpty = bt.isEmpty();
    bool ret1 = bt.search(v);

    bool expr_search = (((isEmpty <= 0) and (min <= (- 1))) or ((ret1 > (- 1)) and (ret1 <= 0) and (min > (- 1))));
        
    if (!expr_search) {
      ceFile << "(search v=" << v
	     << ", min=" << min
	     << ", isEmpty=" << isEmpty
	     << ", ret1=" << ret1
	     << ")\n";
    }
    RC_ASSERT(expr_search);
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
