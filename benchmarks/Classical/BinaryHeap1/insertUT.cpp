#include "../../Impl/BinaryHeapImpl.h"

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << "\n";
    return 1;
  }
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << "\n";
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << "\n";
    return 1;
  }

  bool success = rc::check("testing BinaryHeap insert", [&ceFile](BinaryHeap obj) {
    int i = *rc::gen::inRange(MIN+1, MAX);

    bool isHeap = obj.isHeap();
    int len = obj.len();
    int min = obj.min();

    obj.insert(i);

    bool isHeap1 = obj.isHeap();
    int len1 = obj.len();
    int min1 = obj.min();

    bool expr = ((i <= 0) and (isHeap1 > 0) and ((len1 - len) > 0));

    if (!expr) {
      if (ceFile.is_open()) {
	ceFile << "(insert i = " << i
	       << ", isHeap = " << isHeap
	       << ", len = " << len 
	       << ", isHeap1 = " << isHeap1
	       << ", len1 = " << len1
	       << ")\n" ;
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
