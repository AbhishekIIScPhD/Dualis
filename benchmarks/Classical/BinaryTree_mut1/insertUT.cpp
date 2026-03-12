#include "../../Impl/BinaryTreeImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << "\n";
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << "\n";
    return 1;
  }

  bool success = rc::check("testing BinaryTree insert",[&ceFile](BinaryTree bt) {
    int min = bt.min();
    int isEmpty = bt.isEmpty();
    int n = *rc::gen::inRange(MIN, MAX);
    bt.insert(n);
    int min1 = bt.min();
    int isEmpty1 = bt.isEmpty();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(insert n = " << n
	       << ", min = " << min
	       << ", min1 = " << min1
	       << ", isEmpty = " << isEmpty
	       << ", isEmpty1 = " << isEmpty1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
