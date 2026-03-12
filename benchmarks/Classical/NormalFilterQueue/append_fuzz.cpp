#include "../../FuzzImpl/NormalFilterQueueImpl.h"

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());
    if (fuzzLen < 6) continue;

    NormalFilterQueue nfq;
    init(nfq, fuzzBuf, fuzzLen - 2);

    DECLARE_NFQ_APPEND_STATE_VARS();

    READ_INT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 2, prio);
    READ_INT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 1, packetSize);

    NFQ_APPEND_WITH_STATE(nfq, prio, packetSize);

    bool expr = (false); // Original expression was 'false'

    if (!expr) {
      LOG_NFQ_APPEND_STATE(ceFile, fuzzer_mode);
    }
    fuzzBuf.clear();
    assert(expr);
  }
  ceFile.close();
  return 0;
}
