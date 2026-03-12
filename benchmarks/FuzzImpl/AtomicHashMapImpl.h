#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max
#include <unistd.h>
#include <map>

#define MIN -129
#define MAX 128

#define DECLARE_AHM_INSERT_STATE_VARS()		\
  int32_t k, v;					\
  int len, min, max, kveq;			\
  int len1, min1, max1, kveq1;

#define DECLARE_AHM_INSERT1_STATE_VARS()	\
  int32_t k, v;					\
  int len, min, max, kveq;			\
  int len1, min1, max1, kveq1;

#define DECLARE_AHM_FIND_STATE_VARS()		\
  int32_t k;					\
  int len, min, max, kveq;			\
  int ret1;

#define AHM_INSERT_WITH_STATE(ahm_obj, key_param, val_param)	\
  do {								\
    len = (ahm_obj).len();					\
    min = (ahm_obj).minKey();					\
    max = (ahm_obj).maxKey();					\
    kveq = (ahm_obj).kveq();					\
								\
    (ahm_obj).insert(key_param, val_param);			\
								\
    len1 = (ahm_obj).len();					\
    min1 = (ahm_obj).minKey();					\
    max1 = (ahm_obj).maxKey();					\
    kveq1 = (ahm_obj).kveq();					\
  } while(0)

#define AHM_INSERT1_WITH_STATE(ahm_obj, key_param, val_param)	\
  do {								\
    len = (ahm_obj).len();					\
    min = (ahm_obj).minKey();					\
    max = (ahm_obj).maxKey();					\
    kveq = (ahm_obj).kveq();					\
								\
    (ahm_obj).insert(key_param, val_param);			\
								\
    len1 = (ahm_obj).len();					\
    min1 = (ahm_obj).minKey();					\
    max1 = (ahm_obj).maxKey();					\
    kveq1 = (ahm_obj).kveq();					\
  } while(0)

#define AHM_FIND_WITH_STATE(ahm_obj, key_param)				\
  do {									\
    len = (ahm_obj).len();						\
    min = (ahm_obj).minKey();						\
    max = (ahm_obj).maxKey();						\
    kveq = (ahm_obj).kveq();						\
    									\
    ret1 = (ahm_obj).find(key_param); /* Assuming find returns an int */ \
  } while(0)

#define LOG_AHM_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert k=" << k			\
			  << ", v=" << v			\
			  << ", len=" << len			\
			  << ", min=" << min			\
			  << ", max=" << max			\
			  << ", kveq=" << kveq		\
			  << ", len1=" << len1			\
			  << ", min1=" << min1			\
			  << ", max1=" << max1			\
			  << ", kveq1=" << kveq1		\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_AHM_INSERT1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert1 k=" << k			\
			  << ", v=" << v			\
			  << ", len=" << len			\
			  << ", min=" << min			\
			  << ", max=" << max			\
			  << ", kveq=" << kveq			\
			  << ", len1=" << len1			\
			  << ", min1=" << min1			\
			  << ", max1=" << max1			\
			  << ", kveq1=" << kveq1		\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_AHM_FIND_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(find k=" << k			\
			  << ", len=" << len			\
			  << ", min=" << min			\
			  << ", max=" << max			\
			  << ", kveq = " << kveq		\
			  << ", ret1=" << ret1			\
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


//std::map is an associative container that contains
//key-value pairs with unique keys. Search, insertion, and removal of
//elements have logarithmic complexity.

// inspired form AtomicHashMap from Folly, emplace/insert does not
// rewrite on collision, returns pointer to the exiting (key, value)
// pair, hence using emplace instead of insert on the map.
class HashTable{
private:
  std::map<int, int> ht;
  
public:
  HashTable() = default;

  void insert (int key, int value){
    ht.emplace(key, value);
  }

  void insert1 (int key, int value){
    ht.emplace(key, value);
  }

  int find (int key){
    auto search = ht.find(key);
    return  search != ht.end()? search->second: MAX; // SHRT_MAX means not found
  }

  bool kveq() {
    for (const auto& pair : ht) {
      if (pair.first != pair.second) {
	return false;
      }
    }
    return true;
  }

  int minKey(){
    if(ht.empty()){
      return MAX;
    }
    return ht.begin()->first;
  }

  int maxKey(){
    if(ht.empty()){
      return MIN;
    }
    return ht.rbegin()->first;
  }

  int len(){
    return ht.size();
  }

  int contains(int k){
    return ht.contains(k);
  }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_FIND = 0x02;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init(HashTable &ahm, std::vector<uint8_t>& buf, ssize_t init_len){
  for (ssize_t i = 0; i < init_len;i++) {
    uint8_t command = buf[i] % 3;
    int k, v;

    switch (command) {
    case CMD_INSERT: {
      if ((i+2+1) > init_len) {
	i = init_len;
	break;
      }

      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      READ_INT8_FROM_FUZZBUF(buf, i+2, v);

      ahm.insert(k, v);
      i +=2;
      break;
    }

    case CMD_FIND: {
      if ((i+1+1) > init_len) {
	i = init_len;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);

      ahm.find(k);
      i +=1;
      break;
    }

    default:
      break;
    }
  }
}
