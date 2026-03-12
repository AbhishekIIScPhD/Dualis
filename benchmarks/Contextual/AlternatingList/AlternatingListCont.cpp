#include "../../Impl/AlternatingListImpl.h"

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

  rc::check("testing abstract program", [&ceFile](AlternatingList al) {
    bool flag = true;
    int len = 0;
    int loop_count = *rc::gen::inRange(50, 100);

    for (int i = 0; i < loop_count; ++i) {
      int val;
      if (flag) {
	val = 1;
	int top = al.top();
	len = al.len();

	al.push(val);

	int top1 = al.top();
	int len1 = al.len();

	bool expr_push = (((((top > 10) and (len <= 0)) or (len > 0)) and (val <= 1)) or (val > 1));
	  if (!expr_push) {
	    if (ceFile.is_open()){
	      ceFile << "(push val=" << val
	      << ", top=" << top
	      << ", len=" << len
	      << ", top1=" << top1
	      << ", len1=" << len1
	      << ")\n";
	    }

	  }
	RC_ASSERT(expr_push);

      } else {
	val = 2;
	int top = al.top();
	len = al.len();

	al.push1(val);

	int top1 = al.top();
	int len1 = al.len();

	bool expr_push1 = (((((top > 10) and (len <= 0)) or (len > 0)) and (val <= 1)) or (val > 1));
	  if (!expr_push1) {
	    if (ceFile.is_open()){
	      ceFile << "(push1 val=" << val
	      << ", top=" << top
	      << ", len=" << len
	      << ", top1=" << top1
	      << ", len1=" << len1
	      << ")\n";
	    }

	  }
	RC_ASSERT(expr_push1);
      }

      flag = !flag;
    }
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
