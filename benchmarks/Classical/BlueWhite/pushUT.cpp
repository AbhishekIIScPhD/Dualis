#include "../../Impl/BlueWhiteImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  bool success = rc::check("testing BWList push",[&ceFile](BWList bwl) {
    int bcount = bwl.blue_count();
    int color = *rc::gen::inRange(MIN, MAX);
    bwl.push(color);
    int bcount1 = bwl.blue_count();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
        ceFile  << "(push color ="<< color 
                << ", bcount =" << bcount
                << ", bcount1 =" << bcount1
		<< ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
