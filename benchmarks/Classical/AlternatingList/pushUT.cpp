#include "../../Impl/AlternatingListImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << "\n";
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << "\n";
    return 1;
  }

  bool success = rc::check("testing AlternatingList push",[&ceFile](AlternatingList al) {
    int len = al.len();
    int top = al.top();
    int val = *rc::gen::inRange(MIN+1, MAX);
    al.push(val);
    int len1 = al.len();
    int top1 = al.top();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(push val=" << val
	       << ", top=" << top
	       << ", len=" << len
	       << ", top1=" << top1
	       << ", len1=" << len1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
  
}
