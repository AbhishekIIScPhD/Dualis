#include "../../Impl/BinaryTreeImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing BinaryTree search", [&ceFile](BinaryTree obj) {
    int min = obj.min();
    int isEmpty = obj.isEmpty();
    int v = *rc::gen::inRange(MIN, MAX);
    int ret1 = obj.search(v);
    bool expr = (false);
    if(!expr){
      if(ceFile.is_open()){
        ceFile << "(search v = " << v
	       << ", min = " << min
	       << ", isEmpty = " << isEmpty
	       << ", ret1 = " << ret1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
