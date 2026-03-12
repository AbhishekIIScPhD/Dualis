#include "../../FuzzImpl/BinaryTreeImpl.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint> 
#include <unistd.h>
#include <cstring> // For memcpy

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <log_file>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file: " << filePath << std::endl;
    return 1;
  }

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());

    if (fuzzLen < 4) {
      continue;
    }

    BinaryTree bt;
    uint16_t N_raw;
    std::memcpy(&N_raw, &fuzzBuf[0], sizeof(N_raw));

    const int N = N_raw % 4010;

    for (int i = 0; i < N; ++i) { // min >= 2 
      DECLARE_BT_INSERT_STATE_VARS();
      
      if (i != 4007) {
        n = 2;
      } else {
        n = 1;
      }
      
      BT_INSERT_WITH_STATE(bt, n); // min >= 2 && n >= 1 => min1 >= 2
      /* bool expr_insert = ( n != 1); 
      if (!expr_insert) {
        if (ceFile.is_open()) {
          LOG_BT_INSERT_STATE(ceFile, fuzzer_mode);
        }
      }
      assert(expr_insert); */
    
      int v; int ret1;
      uint16_t v_16;
      std::memcpy(&v_16, &fuzzBuf[2], sizeof(v_16));
      v = v_16;
      bool expr_v = (v != 1);
      if (!expr_v) {
        if (ceFile.is_open()) {
          LOG_BT_SEARCH_STATE(ceFile, fuzzer_mode);
        }
      }
      assert(expr_v);

      if (v < min) {
	      ret1 = false;
      } else {
	      ret1 = true;
      }
      
/*       bool expr_search = (v != 1) || (ret1 == false);

      if (!expr_search) {
        if (ceFile.is_open()) {
          LOG_BT_SEARCH_STATE(ceFile, fuzzer_mode);
        }
      }
      assert(expr_search); */
    }
  }

  ceFile.close();
  return 0;
}
