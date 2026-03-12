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
    int ret1 = list.popHead();
    int len1 = list.len();

    bool expr = ((((len <= 0) and ((len - len1) <= 0)) or ((len - len1) > 0)) and ((len - len1) <= 1));

    if (!expr) {
      if (ceFile.is_open()) {
	ceFile << "(popHead len = " << len
	       << ", ret1 = " << ret1
	       << ", len1 = " << len1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
