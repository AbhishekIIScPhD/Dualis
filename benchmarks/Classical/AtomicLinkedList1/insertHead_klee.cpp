#include "../../KleeImpl/AtomicLinkedListImpl.h"

int main(int argc, char* argv[]) {
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
AtomicLinkedList list) {
    int k = *rc::gen::inRange(MIN, MAX);

    int len = list.len();

    list.insertHead(k);

    int len1 = list.len();

    bool expr = (((len1 - len) <= 1) and ((len1 - len) > 0));

    if (!expr) {
      if (ceFile.is_open()) {
	ceFile << "(insertHead k = " << k
	       << ", len = " << len
	       << ", len1 = " << len1
	       << ")\n" ;
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
