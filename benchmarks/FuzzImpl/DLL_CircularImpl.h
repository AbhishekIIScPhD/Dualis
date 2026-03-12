#include <iostream>
#include <fstream>
#include <vector>
#include <climits>    // for INT_MAX (C++ header)
#include <filesystem>
#include <algorithm>  // Added: For max_element
#include <cstdint>    // Added: For fuzzer types
#include <unistd.h>     // Added: For ssize_t
#include <string>     // Added: For std::string
#include <cassert>

#define MIN -129
#define MAX 128

using namespace std;

#define DECLARE_DLC_PUSH_STATE_VARS()		\
  int val;					\
  int max;					\
  int max1;

#define DLC_PUSH_WITH_STATE(dlc_obj, val_param)	\
  do {						\
    max = (dlc_obj).max();			\
    (dlc_obj).push(val_param);			\
    max1 = (dlc_obj).max();			\
  } while(0)

#define LOG_DLC_PUSH_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(push val ="<< val		\
			  << ", max =" << max			\
			  << ", max1 =" << max1			\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];    \
    target_var_name = read_int8(valptr_##target_var_name);		\
  } while(0)


// --- Transformed Class Definition ---

class DLLCircular {
private:
  // The internal storage is a vector of ints
  std::vector<int> dlc;

public:
  DLLCircular() = default; 

  DLLCircular(std::vector<int> elements) {
    for (auto e : elements) {
      push(e); // This now correctly calls push(int)
    }
  }
    
  // --- FIX ---
  // The 'push' method now correctly accepts a 'int'.
  // This matches the internal vector type and prevents the data-loss bug.
  void push(int key) {
    dlc.push_back(key);
  }

  int max() const {
    if (dlc.empty()){
      return MIN;
    }
    // This logic was already correct
    auto result = *max_element(dlc.cbegin(), dlc.cend());
    return result;
  }

  int len() const {
    return dlc.size();
  }
};

// --- Injected Fuzzer Utilities (Standardized) ---

constexpr uint8_t CMD_PUSH = 0x01;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                                                   \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];     \
    target_var_name = read_uint8(valptr_##target_var_name);              \
  } while(0)

void init(DLLCircular &dlc, std::vector<uint8_t>& buf, ssize_t fuzzLen){
  for (ssize_t i = 0; i < fuzzLen; i++) {
    uint8_t command = buf[i] % 2; 
    int v; // value for insertion

    switch (command) {
    case CMD_PUSH: {
      if ((i+1+1) > fuzzLen) { 
	i = fuzzLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      dlc.push(v); 
      i += 1; 
      break;
    }

    default:
      break;
    }
  }
}
