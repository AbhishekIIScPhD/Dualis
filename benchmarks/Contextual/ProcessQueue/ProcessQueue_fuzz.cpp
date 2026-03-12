#include "../../FuzzImpl/ProcessQueueImpl.h"

int main(int argc, char *argv[]) {
    bool fuzzer_mode = getenv("FUZZING") != nullptr;

    std::string filePath = argv[1];
    
    std::ofstream ceFile(filePath, std::ios::app);

    while (__AFL_LOOP(10000)) {
        std::vector<uint8_t> fuzzBuf(4096);
        ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());

        if (fuzzLen <= 0 || fuzzLen < 2) {
            continue;
        }

        ProcessQueue pq;

        uint8_t loop_count;
        READ_UINT8_FROM_FUZZBUF(fuzzBuf, 1, loop_count);
        int gnexttime = 1;
	unsigned int current_offset = 1; 
        for (int i = 0; i < loop_count; ++i) {
            uint8_t nondet;
            READ_UINT8_FROM_FUZZBUF(fuzzBuf, current_offset, nondet);
            nondet = nondet % 2;
	    current_offset = current_offset % fuzzLen;
            current_offset++;

            if (nondet) {
                DECLARE_PQ_INSERT_STATE_VARS();
                PQ_INSERT_WITH_STATE(pq, gnexttime);

                bool expr_insert = (true);

                if (!expr_insert) {
                    LOG_PQ_INSERT_STATE(ceFile, fuzzer_mode);
                }

                assert(expr_insert);
                gnexttime++;

            } else if (gnexttime > 1) {
                DECLARE_PQ_CHOOSENEXT_STATE_VARS();
                PQ_CHOOSENEXT_WITH_STATE(pq);

                bool expr_choosenext = (true);

                if (!expr_choosenext) {
                    LOG_PQ_CHOOSENEXT_STATE(ceFile, fuzzer_mode);
                }
                assert(expr_choosenext);
                gnexttime--;
            }
        }
        fuzzBuf.clear();
    }
    ceFile.close();
    return 0;
}
