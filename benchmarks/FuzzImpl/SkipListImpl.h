#include <cassert>
#include <unistd.h>
#include <set>
#include <iostream>
#include <limits.h>
#include <filesystem>
#include <fstream>
#include <vector>

#define MIN -129
#define MAX 128 


#define DECLARE_SL_INSERT_STATE_VARS()		\
  int32_t v;					\
  int len, min, max, isPresent;			\
  int len1, min1, max1, isPresent1;


#define DECLARE_SL_REMOVE_STATE_VARS()		\
  int32_t k;					\
  int len, min, max;				\
  int len1, ret1, min1, max1;


#define DECLARE_SL_LOWERBOUND_STATE_VARS()	\
  int32_t k;					\
  int lb_ret1, min, max;

#define SL_INSERT_WITH_STATE(sl_obj, key_param)	\
  do {						\
    v= key_param;				\
    isPresent = (sl_obj).contains(v);		\
    min = (sl_obj).min();			\
    max = (sl_obj).max();			\
    len = (sl_obj).len();			\
    (sl_obj).insert(v);				\
    isPresent1 = (sl_obj).contains(v);		\
    min1 = (sl_obj).min();			\
    max1 = (sl_obj).max();			\
    len1 = (sl_obj).len();			\
  } while(0)


#define SL_INSERT1_WITH_STATE(sl_obj, key_param)	\
  do {							\
    v= key_param;					\
    isPresent = (sl_obj).contains1();			\
    min = (sl_obj).min();				\
    max = (sl_obj).max();				\
    len = (sl_obj).len();				\
    (sl_obj).insert(v);					\
    isPresent1 = (sl_obj).contains1();			\
    min1 = (sl_obj).min();				\
    max1 = (sl_obj).max();				\
    len1 = (sl_obj).len();				\
  } while(0)


#define SL_LOWERBOUND_WITH_STATE(sl_obj, key_param)	\
  do {							\
    k = key_param;					\
    min = (sl_obj).min();				\
    max = (sl_obj).max();				\
    lb_ret1 = (sl_obj).lower_bound(k);			\
  } while(0)

// #define SL_REMOVE_WITH_STATE(sl_obj, key_param)	\
//   do {						\
//     len = (sl_obj).len();			\
//     k = key_param;				\
//     ret1 = (sl_obj).remove(k);			\
//     len1 = (sl_obj).len();			\
//   } while(0)

#define SL_REMOVE_WITH_STATE(sl_obj, key_param)	\
  do {							\
    min = (sl_obj).min();				\
    max = (sl_obj).max();				\
    len = (sl_obj).len();				\
    k = key_param;					\
    ret1 = (sl_obj).remove(k);				\
    min1 = (sl_obj).min();				\
    max1 = (sl_obj).max();				\
    len1 = (sl_obj).len();				\
  } while(0)


#define LOG_SL_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(insert v=" << v					\
	  << ", isPresent="<< isPresent				\
	  << ", min=" << min					\
	  << ", max=" << max					\
	  << ", len=" << len					\
	  << ", isPresent1=" << isPresent1			\
	  << ", min1=" << min1					\
	  << ", max1=" << max1					\
	  << ", len1=" << len1					\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)


#define LOG_SL_LOWERBOUND_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream)						\
	  << "(lower_bound k=" << k					\
	  << ", min=" << min						\
	  << ", max=" << max						\
	  << ", lb_ret1=" << lb_ret1					\
	  << ")\n";							\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)


#define LOG_SL_REMOVE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(remove k =" << k					\
	  << ", min=" << min					\
	  << ", max=" << max					\
	  << ", len =" << len					\
	  << ", min1=" << min1					\
	  << ", max1=" << max1					\
	  << ", len1 =" << len1					\
	  << ", ret1 =" << ret1					\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)


#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_int8(valptr_##target_var_name);		\
  } while(0)

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                                                   \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];     \
    target_var_name = read_uint8(valptr_##target_var_name);              \
  } while(0)

// using set from C++ STL
// set and skiplist have same operations
// set is an associative container implemented using red-black tree.
// associative containers.
class SkipList {
private:
  std::set<int> sl;

public:
  SkipList() = default;
  
  SkipList(const std::vector<int>& initial_data) {
    sl.insert(initial_data.begin(), initial_data.end());
  }

  bool insert(int data) {
    return sl.insert(data).second;
  }

  int isPresent(int data){
    auto search = sl.find(data);
    return  search != sl.end() ? 1 : 0;
  }

  int isOPresent(){
    auto search = sl.find(1);
    return  search != sl.end() ? 1 : 0;
  }

  int remove(int data) {
    auto it = sl.find(data);
    if (it != sl.end()) {
      int removed_value = *it;
      sl.erase(it);
      return removed_value;
    }
    return MIN; // Return MIN if not found
  }

  int upper_bound(int data) {
    auto it = sl.upper_bound(data);
    if (it != sl.end()) {
      return *it;
    }
    return MAX; // Return MAX if no upper bound exists
  }

  int lower_bound(int data) {
    auto it = sl.lower_bound(data);
    if (it != sl.end()) {
      return *it;
    }
 
    return MAX; // Return MAX if no lower bound exists 
  }  
  int min() const {
    if (sl.empty()) {
      return MAX; // Return sentinel value for empty list
    }
    return *sl.begin();
  }

  int max() const {
    if (sl.empty()) {
      return MIN; // Return sentinel value for empty list
    }
    return *sl.rbegin();
  }

  int len() const {
    return sl.size();
  }
    
  int contains(int data) const {
    return sl.count(data) > 0 ? 1 : 0;
  }

  int contains1() const {
    return sl.count(1) > 0 ? 1 : 0;
  }

  const std::set<int>& get_data_set() const {
    return sl;
  }

};


constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_REMOVE = 0x02;
constexpr uint8_t CMD_LOWERBOUND = 0x03;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init(SkipList &sl, std::vector<uint8_t>& buf, ssize_t fuzzLen){
  for (ssize_t i = 0; i < fuzzLen;i++) {
    uint8_t command = buf[i] % 4;
    int k, v;

    switch (command) {
    case CMD_INSERT: {
      if ((i+1+1) > fuzzLen) {
	i = fuzzLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      sl.insert(k);
      i +=1;
      break;
    }
    case CMD_REMOVE:{
      if ((i+1+1) > fuzzLen) {
	i = fuzzLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      sl.remove(k);
      i+=1;
      break;
    }
    case CMD_LOWERBOUND: {
      if ((i+1+1) > fuzzLen) {
	i = fuzzLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      sl.lower_bound(k);
      i +=1;
      break;
    }
    default:
      break;
    }
  }
}
