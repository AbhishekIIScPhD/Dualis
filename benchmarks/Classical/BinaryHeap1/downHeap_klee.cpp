#include "../../KleeImpl/BinaryHeapImpl.h"

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

  prepareData(MIN, MAX);
BinaryHeap obj) {
    obj.insert(*rc::gen::inRange(MIN, MAX));
    obj.downHeap();
    bool isHeap2 = obj.isHeap();

    bool expr = (true);
    if (!expr) {
      if (ceFile.is_open()) {
	ceFile << "(downHeap isHeap2 = " << isHeap2
	       << ")\n" ;
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
