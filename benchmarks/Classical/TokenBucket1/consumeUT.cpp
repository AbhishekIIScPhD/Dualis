#include "../../Impl/TokenBucketImpl.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << std::endl;
    return 1;
  }
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing TokenBucket consume", [&ceFile](TokenBucket bucket) {
    int c_rate = *rc::gen::inRange(1, MAX);
    int b_size = *rc::gen::inRange(1, MAX);
    bucket.generateTokens(b_size);
    int avai_tokens = bucket.getAvailableTokens();
    bool success  = bucket.consume(c_rate);
    int avai_tokens1 = bucket.getAvailableTokens();
	bool expr = (((avai_tokens - avai_tokens1) - c_rate) <= 0);
    if (!expr) {
      ceFile << "(consume c_rate=" << c_rate
	     << ", avai_tokens=" << avai_tokens
	     << ", avai_tokens1=" << avai_tokens1
	     << ")\n";
    }
    RC_ASSERT(expr);
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
