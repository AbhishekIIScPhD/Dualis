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
    if (fuzzLen <= 4) {
      continue;
    }
    BinaryHeap bh;
    uint8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 1, N);
    
    if (N <= 0){
      continue;
    }
    for (int j = 0; j < N; ++j) {
      DECLARE_BH_INSERT1_STATE_VARS();
      i = j;
      BH_INSERT1_WITH_STATE(bh, i);
      bool expr_insert = (true);
      if (!expr_insert) {
	LOG_BH_INSERT1_STATE(ceFile, fuzzer_mode);
      }
      assert(expr_insert);
    }

    int current_len;
    current_len = bh.len();
    while (current_len > 0) {
      {
	DECLARE_BH_DELETEMIN1_STATE_VARS();
	BH_DELETEMIN1_WITH_STATE(bh);
	bool expr_deleteMin = (true);
	if (!expr_deleteMin) {
	  LOG_BH_DELETEMIN1_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_deleteMin);
	current_len = len1;
      }
      {
	DECLARE_BH_DOWNHEAP_STATE_VARS();
	BH_DOWNHEAP_WITH_STATE(bh);
	bool expr_downHeap = (true);
	if (!expr_downHeap) {
	  LOG_BH_DOWNHEAP_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_downHeap);
      }
    }
    fuzzBuf.clear();
  }
  ceFile.close();
  return 0;
}
