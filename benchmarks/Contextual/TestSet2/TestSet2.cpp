#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <numeric>

using namespace std;

class Set {
public:
    std::vector<int> elements;
    void insert(int v) { elements.push_back(v); }
    int computeSum() {
        int s = 0;
        for (int e : elements) s += e;
        return s;
    }
    void clear() { elements.clear(); }
};

// ------------------------------------------------------------------
// STATE LOGGING MACROS (EXACT BINARYTREE PATTERN)
// ------------------------------------------------------------------

#define DECLARE_SET_STATE_VARS() \
    int pisEmpty; int pisEmpty1; \
    int nisEmpty; int nisEmpty1; \
    int pmin;     int pmin1;     \
    int nmax;     int nmax1;     \
    int psum;     int psum1;     \
    int nsum;     int nsum1;     \
    int v1;       int flag;

#define SET_INSERT_POS_WITH_STATE(set_obj, val, cur_pmin, cur_psum, cur_pisEmpty) \
    do { \
        v1 = (val); pmin = (cur_pmin); psum = (cur_psum); pisEmpty = (cur_pisEmpty); \
        flag = 0; \
        (set_obj).insert(v1); \
        pisEmpty1 = 0; psum1 = psum + v1; pmin1 = (v1 < pmin) ? v1 : pmin; \
        (cur_pmin) = pmin1; (cur_psum) = psum1; (cur_pisEmpty) = pisEmpty1; \
    } while(0)

#define SET_INSERT_NEG_WITH_STATE(set_obj, val, cur_nmax, cur_nsum, cur_nisEmpty) \
    do { \
        v1 = (val); nmax = (cur_nmax); nsum = (cur_nsum); nisEmpty = (cur_nisEmpty); \
        flag = 1; \
        (set_obj).insert(v1); \
        nisEmpty1 = 0; nsum1 = nsum + v1; nmax1 = (v1 > nmax) ? v1 : nmax; \
        (cur_nmax) = nmax1; (cur_nsum) = nsum1; (cur_nisEmpty) = nisEmpty1; \
    } while(0)

#define SET_COMPUTE_SUM1_WITH_STATE(set_obj, cur_psum, res_sum1) \
    do { \
        psum = (cur_psum); flag = 2; \
        res_sum1 = (set_obj).computeSum(); \
        psum1 = res_sum1; \
    } while(0)

#define SET_COMPUTE_SUM2_WITH_STATE(set_obj, cur_nsum, res_sum2) \
    do { \
        nsum = (cur_nsum); flag = 3; \
        res_sum2 = (set_obj).computeSum(); \
        nsum1 = res_sum2; \
    } while(0)

#define LOG_SET_INSERT_POS_STATE(log_file, is_fuzzer) \
  do { if (!(is_fuzzer) && (log_file).is_open()) { \
    (log_file) << "(insert1 flag = " << flag << ", pmin = " << pmin << ", pmin1 = " << pmin1 << ")\n"; \
    (log_file).flush(); } } while(0)

#define LOG_SET_INSERT_NEG_STATE(log_file, is_fuzzer) \
  do { if (!(is_fuzzer) && (log_file).is_open()) { \
    (log_file) << "(insert2 flag = " << flag << ", nmax = " << nmax << ", nmax1 = " << nmax1 << ")\n"; \
    (log_file).flush(); } } while(0)

#define LOG_SET_SUM1_STATE(log_file, is_fuzzer) \
  do { if (!(is_fuzzer) && (log_file).is_open()) { \
    (log_file) << "(computeSum1 flag = " << flag << ", psum = " << psum << ", psum1 = " << psum1 << ")\n"; \
    (log_file).flush(); } } while(0)

#define LOG_SET_SUM2_STATE(log_file, is_fuzzer) \
  do { if (!(is_fuzzer) && (log_file).is_open()) { \
    (log_file) << "(computeSum2 flag = " << flag << ", nsum = " << nsum << ", nsum1 = " << nsum1 << ")\n"; \
    (log_file).flush(); } } while(0)

// ------------------------------------------------------------------
// MAIN HARNESS
// ------------------------------------------------------------------

int main(int argc, char *argv[]) {
    bool fuzzer_mode = getenv("FUZZING") != nullptr;
    if (argc < 2) return 1;
    std::string filePath = argv[1];
    std::ofstream ceFile(filePath, std::ios::app);
    Set sPos, sNeg;

    while (__AFL_LOOP(10000)) {
        sPos.clear(); sNeg.clear();
        int cur_pmin = 127; int cur_psum = 0; int cur_pisEmpty = 1;
        int cur_nmax = -128; int cur_nsum = 0; int cur_nisEmpty = 1;

        std::vector<uint8_t> fuzzBuf(4096);
        ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());
        if (fuzzLen < 3) continue; // 2 for N, 1 for at least one int8_t

        uint16_t N_raw;
        std::memcpy(&N_raw, &fuzzBuf[0], sizeof(N_raw));
        const int N = N_raw % 2000;
        size_t offset = 2;

        for (int i = 0; i < N; ++i) {
            if (offset + 1 > (size_t)fuzzLen) break;
            
            int8_t val_in; // CHANGED TO INT8_T
            std::memcpy(&val_in, &fuzzBuf[offset], sizeof(val_in));
            offset += 1;
            
            DECLARE_SET_STATE_VARS();

            if (val_in >= 0) {
                SET_INSERT_POS_WITH_STATE(sPos, val_in, cur_pmin, cur_psum, cur_pisEmpty);
                bool expr_insert1 = (pmin1 <= pmin);
                if (!expr_insert1) { LOG_SET_INSERT_POS_STATE(ceFile, fuzzer_mode); }
                assert(expr_insert1);
            } else {
                SET_INSERT_NEG_WITH_STATE(sNeg, val_in, cur_nmax, cur_nsum, cur_nisEmpty);
                bool expr_insert2 = (nmax1 >= nmax);
                if (!expr_insert2) { LOG_SET_INSERT_NEG_STATE(ceFile, fuzzer_mode); }
                assert(expr_insert2);
            }
        }

        // --- computeSum1 Block ---
        DECLARE_SET_STATE_VARS();
        int final_sum1;
        SET_COMPUTE_SUM1_WITH_STATE(sPos, cur_psum, final_sum1);
        bool expr_sum1 = (cur_pisEmpty || final_sum1 >= 0);
        if (!expr_sum1) { LOG_SET_SUM1_STATE(ceFile, fuzzer_mode); }
        assert(expr_sum1);

        // --- computeSum2 Block ---
        DECLARE_SET_STATE_VARS();
        int final_sum2;
        SET_COMPUTE_SUM2_WITH_STATE(sNeg, cur_nsum, final_sum2);
        bool expr_sum2 = (cur_nisEmpty || final_sum2 < 0);
        if (!expr_sum2) { LOG_SET_SUM2_STATE(ceFile, fuzzer_mode); }
        assert(expr_sum2);

        bool final_check = ((cur_nisEmpty || final_sum2 < 0) && (cur_pisEmpty || final_sum1 >= 0));
        assert(final_check);
    }
    ceFile.close();
    return 0;
}