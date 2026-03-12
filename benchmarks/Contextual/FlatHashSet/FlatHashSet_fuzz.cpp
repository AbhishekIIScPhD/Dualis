#include "../../FuzzImpl/FlatHashSetImpl.h"
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

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());

    if (fuzzLen < 4) {
      continue;
    }

    FlatHashSet fhs;

    int N_val;
    READ_INT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -1, N_val);

    if (N_val <= 0 || N_val >= 128){
      continue;
    }
        
    for (int i = 0; i < N_val; ++i) {
        DECLARE_FHS_INSERT_STATE_VARS();
        FHS_INSERT_WITH_STATE(fhs, i);

        bool expr_insert = (false);
        
        if (!expr_insert) {
          LOG_FHS_INSERT_STATE(ceFile, fuzzer_mode);
        }
        assert(expr_insert);
    }
        
    for (int i = 0; i < N_val; ++i) {
        DECLARE_FHS_ERASE_STATE_VARS();
        FHS_ERASE_WITH_STATE(fhs, i);

        bool expr_erase = (false);
        if (!expr_erase) {
          LOG_FHS_ERASE_STATE(ceFile, fuzzer_mode);
        }
        assert(expr_erase);
    }

    {
      DECLARE_FHS_RESERVE_STATE_VARS();
      FHS_RESERVE_WITH_STATE(fhs, N_val);

      bool expr_reserve = (false);
      if(!expr_reserve) {
        LOG_FHS_RESERVE_STATE(ceFile, fuzzer_mode);
      }
      assert(expr_reserve);
    }

    for (int i = 0; i < N_val; ++i) {
        DECLARE_FHS_INSERT_STATE_VARS();
        FHS_INSERT_WITH_STATE(fhs, (i + N_val)%128);

        bool expr_insert1 = (false);
        
        if (!expr_insert1) {
          LOG_FHS_INSERT1_STATE(ceFile, fuzzer_mode);
        }
        assert(expr_insert1);
    }
  }
  ceFile.close();
  return 0;
}
