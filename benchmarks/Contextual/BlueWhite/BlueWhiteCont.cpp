#include "../../Impl/BlueWhiteImpl.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << std::endl;
    return 1;
  }
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing abstract color program", [&ceFile](BWList bwl) {
    bool inserted_blue = false;
    int bcount = 0;
    int N = *rc::gen::inRange(5, 100);

    for (int i = 0; i < N; ++i) {
      int color = *rc::gen::elementOf(std::vector<int>{0, 1});
      if (color == 0 && !inserted_blue) {
	int bcount = bwl.blue_count();
	bwl.push(color);
	int bcount1 = bwl.blue_count();
	bool expr_push = (false);
	if (!expr_push) {
	  ceFile << "(push color=" << color
		 << ", bcount=" << bcount 
		 << ", bcount1=" << bcount1
		 << ")\n";
	}
	RC_ASSERT(expr_push);
	inserted_blue = bwl.blue_present();
      } else if (color == 1) {
	int bcount = bwl.blue_count();
	bwl.push1(color);
	int bcount1 = bwl.blue_count();
	bool expr_push1 = (false);
	if (!expr_push1) {
	  ceFile << "(push1 color=" << color
		 << ", bcount=" << bcount
		 << ", bcount1=" << bcount1
		 << ")\n";
	}
	RC_ASSERT(expr_push1);
      }
    }
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
