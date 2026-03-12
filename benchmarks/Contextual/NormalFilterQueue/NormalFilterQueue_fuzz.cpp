#include "../../FuzzImpl/NormalFilterQueueImpl.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint> // For uint8_t
#include <unistd.h>  // For read


int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;

  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  // AFL persistent loop
  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());

    if (fuzzLen < 5) {
      continue;
    }

    NormalFilterQueue nfq;

    uint8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 1, N);

    if (N <= 0) {
      continue;
    }
        
    if (static_cast<size_t>((N * 2) + 2) >= static_cast<size_t>(fuzzLen)) {
      continue;
    }

    unsigned int current_offset = 2;
        
    for (int i = 0; i < N; ++i) {
           
      int8_t packetSize_val;
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, current_offset, packetSize_val);
      current_offset++; // Advance offset
           
      uint8_t prio_raw;
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, current_offset, prio_raw);
      current_offset++; // Advance offset

      int prio_val = prio_raw % 2 ;

      if (packetSize_val > 0) {
	if (prio_val == 0 && packetSize_val >= 50) {
                    
	  DECLARE_NFQ_APPEND_STATE_VARS();
	  NFQ_APPEND_WITH_STATE(nfq, prio_val, packetSize_val);

	  bool expr_append = (false);                    
	  if (!expr_append) {
	    LOG_NFQ_APPEND_STATE(ceFile, fuzzer_mode);
	  }
	  assert(expr_append);
	}
      }

      {
	DECLARE_NFQ_PROCESSQUEUE_STATE_VARS();
	NFQ_PROCESSQUEUE_WITH_STATE(nfq);

	bool expr_processQueue = (false);
                
	if (!expr_processQueue) {
	  LOG_NFQ_PROCESSQUEUE_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_processQueue);
      }
    }
  }

  ceFile.close();
  return 0;
}
