#include "../../Impl/StackImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  bool success = rc::check("testing Stack pop", [&ceFile](Stack obj) {
    int sl = obj.len();
    obj.pop();
    int sl1 = obj.len();
    bool expr = (((sl - sl1) <= 1) and ((sl - sl1) > 0));
    if(not expr){
      if (ceFile.is_open()){
	ceFile << "(po sl = " << sl
	       << ", sl1 = " << sl1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
