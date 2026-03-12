#include "../../Impl/AtomicLinkedListImpl.h"

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

  bool success = rc::check("testing AtomicLinkedList popHead", [&ceFile](AtomicLinkedList list) {
    int len = list.len();
    int max1 = list.max();
    int ret1 = list.popHead();
    int len1 = list.len();

    bool expr = (false);

    if (!expr) {
      if (ceFile.is_open()) {
	ceFile << "(popHead len = " << len
         << ", len1 = " << len1
         << ", max1 = " << max1
	       << ", ret1 = " << ret1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
