#include "../../Impl/RedBlackTreeImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }
  
  bool success = rc::check("testing RedBlackTree insert",[&ceFile](RedBlackTree rbt) {
    int max = rbt.max();
    int min = rbt.min();
    int len = rbt.len();
    int noDup = rbt.noDup();
    short data = *rc::gen::inRange(MIN+1, MAX);
    int ret1 = rbt.search(data);
    bool expr = (true);
    if(!expr){
      if (ceFile.is_open()){
        ceFile << "(search data = " << data
	       << ", max = " << max
	       << ", min = " << min
	       << ", len = " << len
	       << ", noDup = " << noDup
	       << ", ret1 = " << ret1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
