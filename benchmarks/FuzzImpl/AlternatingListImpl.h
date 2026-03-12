#include <iostream>
#include <fstream>
#include <vector>
#include <climits> // for INT_MAX (C++ header)
#include <filesystem>
#include <stack>
#include <cstdint> // Added for fuzzer types
#include <unistd.h> // Added for ssize_t
#include <string> // Added for std::string
#include <algorithm> // Added for std::min/max
#include <cassert>

using namespace std;

#define MIN -129
#define MAX 128

#define DECLARE_AL_PUSH_STATE_VARS()		\
  int val;					\
  int top, len;					\
  int top1, len1;

#define DECLARE_AL_PUSH1_STATE_VARS()		\
  int val;					\
  int top, len;					\
  int top1, len1;

#define AL_PUSH_WITH_STATE(al_obj, val_param)	\
  do {						\
    len = (al_obj).len();			\
    top = (al_obj).top();			\
						\
    (al_obj).push(val_param);			\
						\
    len1 = (al_obj).len();			\
    top1 = (al_obj).top();			\
  } while(0)

#define AL_PUSH1_WITH_STATE(al_obj, val_param)	\
  do {						\
    len = (al_obj).len();			\
    top = (al_obj).top();			\
						\
    (al_obj).push(val_param);			\
						\
    len1 = (al_obj).len();			\
    top1 = (al_obj).top();			\
  } while(0)

#define LOG_AL_PUSH_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(push val=" << val		\
			  << ", top=" << top			\
			  << ", len=" << len			\
			  << ", top1=" << top1			\
			  << ", len1=" << len1			\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_AL_PUSH1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(push1 val=" << val		\
			  << ", top=" << top			\
			  << ", len=" << len			\
			  << ", top1=" << top1			\
			  << ", len1=" << len1			\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)


#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_int8(valptr_##target_var_name);		\
  } while(0)

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_uint8(valptr_##target_var_name);		\
  } while(0)

// --- Transformed Class Definition ---

class AlternatingList {
private:
  std::vector<int> al;

public:
  AlternatingList() = default; // Added default constructor

  AlternatingList(std::vector<int> elements) {
    for (auto e : elements) {
      push(e);
    }
  }
 
  void push(int key) {
    al.push_back(key);
  }

  void push1(int key) {
    al.push_back(key);
  }

  int top() const{
    for (auto it = al.rbegin(); it != al.rend(); ++it) {
      if (*it == 1 || *it == 2) {
        return *it; // Found the most recent 1 or 2
      }
    }
    return MAX;
  }

  int len() const{
    int count = 0;
    for (int val : al) {
      if (val == 1 || val == 2) {
        count++;
      }
    }
    return count;
  }
};

constexpr uint8_t CMD_PUSH = 0x01;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init(AlternatingList &al, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen; i++) {
    uint8_t command = buf[i] % 2; // 0=default, 1=push
    int32_t v; // value for push

    switch (command) {
    case CMD_PUSH: {
      if ((i+1+1) > initLen) { // Need 1 byte for parameter
	i = initLen;
	break;
      }
      READ_UINT8_FROM_FUZZBUF(buf, i+1, v);
      al.push(v);
      i += 1; 
      break;
    }

    default:
      break;
    }
  }
}
