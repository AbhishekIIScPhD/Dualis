#include <iostream>
#include <fstream>
#include <vector>
#include <climits> // for INT_MAX (C++ header)
#include <filesystem>
#include <stdexcept> // For std::out_of_range
#include <algorithm>
#include <functional>
#include <cstdint>   // Added for fuzzer types
#include <unistd.h>    // Added for ssize_t
#include <string>    // Added for std::string
#include <cassert>

using namespace std;

#define MIN -129
#define MAX 128

#define DECLARE_ALL_INSERTHEAD_STATE_VARS()	\
  int k;					\
  int len;					\
  int len1;

#define DECLARE_ALL_INSERTHEAD1_STATE_VARS()	\
  int k;					\
  int len, minn, max;				\
  int len1, minn1, max1;

#define DECLARE_ALL_POPHEAD_STATE_VARS()	\
  int len;					\
  int ret1;					\
  int max1;					\
  int len1;

#define DECLARE_ALL_POPHEAD1_STATE_VARS()	\
  int len, max, minn;				\
  int ret1, max1, minn1, len1;			\

#define ALL_INSERTHEAD_WITH_STATE(all_obj, key_param)	\
  do {							\
    len = (all_obj).len();				\
    (all_obj).insertHead(key_param);			\
    len1 = (all_obj).len();				\
  } while(0)

#define ALL_INSERTHEAD1_WITH_STATE(all_obj, key_param)	\
  do {							\
    minn = (all_obj).min();				\
    max = (all_obj).max();				\
    len = (all_obj).len();				\
    (all_obj).insertHead(key_param);			\
    minn1 = (all_obj).min();				\
    max1 = (all_obj).max();				\
    len1 = (all_obj).len();				\
  } while(0)

#define ALL_POPHEAD_WITH_STATE(all_obj)		\
  do {						\
    len = (all_obj).len();			\
    ret1 = (all_obj).popHead();			\
    max1 = (all_obj).max();			\
    len1 = (all_obj).len();			\
  } while(0)

#define ALL_POPHEAD1_WITH_STATE(all_obj)	\
  do {						\
    minn = (all_obj).min();			\
    max = (all_obj).max();			\
    len = (all_obj).len();			\
    ret1 = (all_obj).popHead();			\
    minn1 = (all_obj).min();			\
    max1 = (all_obj).max();			\
    len1 = (all_obj).len();			\
  } while(0)

#define LOG_ALL_INSERTHEAD_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream) << "(insertHead k = " << k			\
			  << ", len = " << len				\
			  << ", len1 = " << len1			\
			  << ")\n" ;					\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)

#define LOG_ALL_INSERTHEAD1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream) << "(insertHead k = " << k			\
			  << ", min = " << minn				\
			  << ", max = " << max				\
			  << ", len = " << len				\
			  << ", min1 = " << minn1			\
			  << ", max1 = " << max1			\
			  << ", len1 = " << len1			\
			  << ")\n" ;					\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)

#define LOG_ALL_POPHEAD1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(popHead min = " << minn		\
			  << ", max = " << max			\
			  << ", len = " << len			\
			  << ", min1 = " << minn1		\
			  << ", max1 = " << max1		\
			  << ", len1 = " << len1		\
			  << ", ret1 = " << ret1		\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_ALL_POPHEAD_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(popHead len = " << len		\
			  << ", ret1 = " << ret1		\
			  << ", len1 = " << len1		\
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


class AtomicLinkedList {
public:
  std::vector<int> all;

  AtomicLinkedList() = default;

  AtomicLinkedList(std::vector<int> elements) {
    all = elements;
  }

  void insertHead(int key) {
    all.insert(all.begin(), key);
  }

  int popHead() {
    if (all.empty()) {
      return MAX; 
    }
    int head_val = all.front();
    all.erase(all.begin());
    return head_val;
  }

  int getHead() const {
    if (all.empty()) {
      return MAX;
    }
    return all.front();
  }

  int len() const {
    return all.size();
  }

  int max(){
    if(all.empty()){
      return MIN;
    }
    int max_val = *std::max_element(all.begin(), all.end());
    return max_val;
  }

  int min(){
    if(all.empty()){
      return MAX;
    }
    int min_val = *std::min_element(all.begin(), all.end());
    return min_val;
  }
};

constexpr uint8_t CMD_INSERTHEAD = 0x01;
constexpr uint8_t CMD_POPHEAD = 0x02;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init(AtomicLinkedList &all, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen; i++) {
    uint8_t command = buf[i] % 3; // 0=default, 1=insertHead, 2=popHead
    int v; // value for insertion

    switch (command) {
    case CMD_INSERTHEAD: {
      if ((i+1+1) > initLen) { // Need 1 byte for parameter
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      all.insertHead(v);
      i += 1; // Consumed 1 param byte
      break;
    }

    case CMD_POPHEAD: {
      all.popHead();
      break;
    }

    default:
      break;
    }
  }
}
