#include "../../FuzzImpl/PriorityFilterQueueImpl.h"

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());
    if (fuzzLen < 4) continue;

    PriorityFilterQueue pfq;
    init(pfq, fuzzBuf, fuzzLen -2); 

    DECLARE_PFQ_PROCESSQUEUE_STATE_VARS();

    PFQ_PROCESSQUEUE_WITH_STATE(pfq);

    bool expr = (false);

    if (!expr) {
      LOG_PFQ_PROCESSQUEUE_STATE(ceFile, fuzzer_mode);
    }
    fuzzBuf.clear();
    assert(expr);
  }

  ceFile.close();
  return 0;
}
