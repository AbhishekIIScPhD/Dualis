#include "../../FuzzImpl/AtomicLinkedListImpl.h"

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << std::endl;
    return 1;
  }
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());
    if (fuzzLen <= 10) {
      continue;
    }
 
    AtomicLinkedList all;
        
    uint8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -1, N);
    if(N <= 0){
      continue;
    }
    for (int i = 0; i < N; ++i) {
      DECLARE_ALL_INSERTHEAD1_STATE_VARS();
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 2, k);
      if (k == i){
	ALL_INSERTHEAD1_WITH_STATE(all, i); 
	bool expr_insertHead = (true);
	if (!expr_insertHead) {
	  LOG_ALL_INSERTHEAD1_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_insertHead);
      }
    }

    int current_len = all.len(); 
    while (current_len > 0) {
      DECLARE_ALL_POPHEAD1_STATE_VARS();
      ALL_POPHEAD1_WITH_STATE(all);
      bool expr_popHead = (true);
      if (!expr_popHead) {
	LOG_ALL_POPHEAD1_STATE(ceFile, fuzzer_mode);
      }
      assert(expr_popHead);
      current_len = len1;
    }     
    fuzzBuf.clear();
  }
  ceFile.close();
  return 0;
}
