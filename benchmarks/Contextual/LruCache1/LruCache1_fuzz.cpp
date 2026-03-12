#include "../../FuzzImpl/LruCacheImpl.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint> // For uint8_t
#include <unistd.h>  // For read

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

    uint8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -1, N);

    LRUCache lruc(N);

    for (int i = 0; i < N; ++i) {
      {
	DECLARE_LRU_INSERTORASSIGN_STATE_VARS();
	k = i%128;
	v = i%128;
	LRU_INSERTORASSIGN_WITH_STATE(lruc, k, v);

	bool expr_insert_or_assign = (false);
	if (!expr_insert_or_assign) {
	  LOG_LRU_INSERTORASSIGN_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_insert_or_assign);
      }
      {
	DECLARE_LRU_FIND_STATE_VARS();
	READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 5, k);
	LRU_FIND_WITH_STATE(lruc, k%128);
	bool expr_find = (false);
	if (!expr_find) {
	  LOG_LRU_FIND_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_find);
      }
    }
    fuzzBuf.clear();
  }
  ceFile.close();
  return 0;
}
