#include "../../KleeImpl/DLL_CircularImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  prepareData(MIN, MAX);
DLLCircular dllc) {
    int max = dllc.max();
    int val = *rc::gen::inRange(MIN, MAX);
    dllc.push(val);
    int max1 = dllc.max();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
        ceFile << "(push val ="<< val
	       << ", max =" << max 
	       << ", max1 =" << max1
	       << ")\n";
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
