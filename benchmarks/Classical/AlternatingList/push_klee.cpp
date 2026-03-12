#include "../../KleeImpl/AlternatingListImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << "\n";
  std::ofstream ceFile(filePath, std::ios::out); 
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << "\n";
    return 1;
  }

  prepareData(MIN, MAX);
  AlternatingList al;
  init(al);

  int len = al.len();
  int top = al.top();
  int val = getValue();
  al.push(val);
  int len1 = al.len();
  int top1 = al.top();
  bool expr = (false);

  if(!expr){
    if (ceFile.is_open()){
      ceFile << "(push val=" << klee_get_value_i32(val)
	     << ", top=" << klee_get_value_i32(val)
	     << ", len=" << klee_get_value_i32(len)
	     << ", top1=" << klee_get_value_i32(top1)
	     << ", len1=" << klee_get_value_i32(len1)
	     << ")\n";
    }
  }
  klee_assert(expr);

  ceFile.close();
  return 0;
  
}
