#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

using namespace std;

// ------------------------------------------------------------------
// State Logging Macros
// ------------------------------------------------------------------

#define DECLARE_FOO_STATE_VARS() \
  int x;                         \
  int x1;                        \
  int flag;

#define FOO_WITH_STATE(action_param, target_var) \
  do {                                           \
    x = (target_var);                            \
    flag = (action_param);                       \
                                                 \
    foo(action_param, target_var);               \
                                                 \
    x1 = (target_var);                           \
  } while(0)

#define LOG_FOO1_STATE(log_file_stream, is_fuzzer_mode)                   \
  do {                                                                   \
    if (!(log_file_stream)) {                                            \
      std::cerr << "Error: Unable to open log file." << "\n";            \
      exit(1);                                                           \
    }                                                                    \
    if (!(is_fuzzer_mode)) {                                             \
      if ((log_file_stream).is_open()) {                                 \
        (log_file_stream) << "(foo1 flag = " << flag                          \
                          << ", x = " << x                               \
                          << ", x1 = " << x1                             \
                          << ")\n";                                      \
        (log_file_stream).flush();                                       \
      }                                                                  \
    }                                                                    \
  } while(0)

#define LOG_FOO2_STATE(log_file_stream, is_fuzzer_mode)                   \
  do {                                                                   \
    if (!(log_file_stream)) {                                            \
      std::cerr << "Error: Unable to open log file." << "\n";            \
      exit(1);                                                           \
    }                                                                    \
    if (!(is_fuzzer_mode)) {                                             \
      if ((log_file_stream).is_open()) {                                 \
        (log_file_stream) << "(foo2 flag = " << flag                          \
                          << ", x = " << x                               \
                          << ", x1 = " << x1                             \
                          << ")\n";                                      \
        (log_file_stream).flush();                                       \
      }                                                                  \
    }                                                                    \
  } while(0)

// ------------------------------------------------------------------
// Target Function
// ------------------------------------------------------------------

void foo(int action, int& x_ref) {
    if (action == 0) {
        x_ref += 1;
    } else if (action == 1) {
        x_ref -= 1;
    }
}

// ------------------------------------------------------------------
// Main Fuzzing Harness
// ------------------------------------------------------------------

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
        std::vector<uint8_t> buffer(4096);
        ssize_t bytes_read = read(0, buffer.data(), buffer.size());

        if (bytes_read < 4) {
            continue;
        }

        uint16_t N_raw;
        std::memcpy(&N_raw, &buffer[0], sizeof(N_raw));
        
        int orig_x = 0; 
        unsigned int max_safe_N = (bytes_read >= 2) ? (bytes_read - 2) / 2 : 0;

        uint16_t N = N_raw;
        if (N > max_safe_N) {
            N = max_safe_N; 
        }

        size_t current_offset = 2;
        for (unsigned int i = 0; i < N; i++) {
            int16_t v;
            std::memcpy(&v, &buffer[current_offset], sizeof(v));
            current_offset += 2;

            if (v) {
                DECLARE_FOO_STATE_VARS();
                x = orig_x; 
                
                FOO_WITH_STATE(0, x);

                orig_x = x; 
                
                bool expr_foo1= (x != 10000);
                if (!expr_foo1) {
                    if (ceFile.is_open()) {
                        LOG_FOO1_STATE(ceFile, fuzzer_mode);
                    }
                }
                assert(expr_foo1);
                
            } else {
                DECLARE_FOO_STATE_VARS();
                x = orig_x;

                FOO_WITH_STATE(1, x);
                
                orig_x = x;                 

                bool expr_foo2 = (x != 10000);
                if (!expr_foo2) {
                    if (ceFile.is_open()) {
                        LOG_FOO2_STATE(ceFile, fuzzer_mode);
                    }
                }
                assert(expr_foo2);
            }
        }
    }

    ceFile.close();
    return 0;
}