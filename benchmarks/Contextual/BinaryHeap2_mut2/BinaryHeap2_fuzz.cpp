#include "../../FuzzImpl/BinaryHeapImpl.h"

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
    BinaryHeap bh;
    uint8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 1, N);
    if (N <= 0){
      continue;
    }
    for (int j = 0; j < N; ++j) {
      DECLARE_BH_INSERT_STATE_VARS();
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 2, i);
      if( i == j){
	BH_INSERT_WITH_STATE(bh, i); 
	bool expr_insert = (true);
	if (!expr_insert) {
	  LOG_BH_INSERT_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_insert);
      }
    }
    {
      DECLARE_BH_DELETEMIN_STATE_VARS();
      BH_DELETEMIN_WITH_STATE(bh);
      bool expr_deleteMin = (true); 
      if (!expr_deleteMin) {
	LOG_BH_DELETEMIN_STATE(ceFile, fuzzer_mode);
      }
      assert(expr_deleteMin);
    }
    {
      DECLARE_BH_DOWNHEAP_STATE_VARS();
      BH_DOWNHEAP_WITH_STATE(bh);
      bool expr_downheap = (isHeap2 > 0);
      if (!expr_downheap) {
	LOG_BH_DOWNHEAP_STATE(ceFile, fuzzer_mode);
      }
      assert(expr_downheap);
    }
    fuzzBuf.clear();
  }
  ceFile.close();
  return 0;
}
