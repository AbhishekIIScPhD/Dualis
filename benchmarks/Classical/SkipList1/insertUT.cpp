#include "../../Impl/SkipListImpl.h"

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());
    if (fuzzLen <= 0) continue;

    if (fuzzBuf.size() < 4) continue;
    SkipList sl;
    init(sl, fuzzBuf);

    DECLARE_SL_INSERT_STATE_VARS();

    READ_INT8_FROM_FUZZBUF(fuzzBuf, 1, i);

    SL_INSERT_WITH_STATE(sl, i);
      
    bool expr = ((i <= (- 1)) and (isPresent1 > 0));

    if (!expr) {
        LOG_SL_INSERT_STATE(ceFile, fuzzer_mode);
    }
    fuzzBuf.clear();
    assert(expr);  
  }
  ceFile.close();
  return 0;
}
