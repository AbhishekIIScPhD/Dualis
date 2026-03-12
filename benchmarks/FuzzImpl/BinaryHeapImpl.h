#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <stdexcept> // For std::out_of_range

#include <algorithm>
#include <functional>
#include <cassert>
#include <unistd.h>

#define MIN -129
#define MAX 128

using namespace std;

#define DECLARE_BH_INSERT_STATE_VARS()		\
  int32_t i;					\
  bool isHeap;int len, min;			\
  bool isHeap1;int len1, min1;			\

#define DECLARE_BH_INSERT1_STATE_VARS()		\
  int32_t i;					\
  bool isHeap;int len, min, max;		\
  bool isHeap1;int len1, min1, max1;

#define DECLARE_BH_DOWNHEAP_STATE_VARS()	\
  bool isHeap2;					\

#define DECLARE_BH_DELETEMIN_STATE_VARS()	\
  int len, min; bool isHeap;			\
  int ret1, len1, min1; bool isHeap1;		\

#define DECLARE_BH_DELETEMIN1_STATE_VARS()	\
  int len, min, isHeap, max;			\
  int ret1, len1, min1, isHeap1, max1;		\

#define BH_INSERT_WITH_STATE(bh_obj, key)	\
  do{						\
    i = key;					\
    isHeap = (bh_obj).isHeap();			\
    len = (bh_obj).len();			\
    min = (bh_obj).min();			\
    						\
    (bh_obj).insert(i);				\
						\
    isHeap1 = (bh_obj).isHeap();		\
    len1 = (bh_obj).len();			\
    min1 = (bh_obj).min();			\
  }while(0)

#define BH_INSERT1_WITH_STATE(bh_obj, key)	\
  do{						\
    i = key;					\
    isHeap = (bh_obj).isHeap();			\
    len = (bh_obj).len();			\
    min = (bh_obj).min();			\
    max = (bh_obj).min();			\
    (bh_obj).insert(i);				\
						\
    isHeap1 = (bh_obj).isHeap();		\
    len1 = (bh_obj).len();			\
    min1 = (bh_obj).min();			\
    max1 = (bh_obj).min();			\
  }while(0)

#define BH_DELETEMIN_WITH_STATE(bh_obj)		\
  do{						\
    len = (bh_obj).len();			\
    min = (bh_obj).min();			\
    isHeap = (bh_obj).isHeap();			\
    ret1 = (bh_obj).deleteMin();		\
    len1 = (bh_obj).len();			\
    min1 = (bh_obj).min();			\
    isHeap1 = (bh_obj).isHeap();		\
  }while(0)

#define BH_DELETEMIN1_WITH_STATE(bh_obj)	\
  do{						\
    len = (bh_obj).len();			\
    min = (bh_obj).min();			\
    max = (bh_obj).min();			\
    isHeap = (bh_obj).isHeap();			\
    ret1 = (bh_obj).deleteMin();		\
    len1 = (bh_obj).len();			\
    min1 = (bh_obj).min();			\
    max1 = (bh_obj).min();			\
    isHeap1 = (bh_obj).isHeap();		\
  }while(0)

#define BH_DOWNHEAP_WITH_STATE(bh_obj)		\
  do{						\
    (bh_obj).downHeap();			\
    isHeap2 = (bh_obj).isHeap();		\
  }while(0)

#define LOG_BH_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert i = " << i		\
			  << ", isHeap = " << isHeap		\
			  << ", len = " << len			\
			  << ", isHeap1 = " << isHeap1		\
			  << ", len1 = " << len1		\
			  << ")\n";				\
      }								\
      (log_file_stream).flush();				\
    }								\
  } while(0)

#define LOG_BH_INSERT1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert i = " << i		\
			  << ", isHeap = " << isHeap		\
			  << ", min = " << min			\
			  << ", max = " << max			\
			  << ", len = " << len			\
			  << ", isHeap1 = " << isHeap1		\
			  << ", min1 = " << min1		\
			  << ", max1 = " << max1		\
			  << ", len1 = " << len1		\
			  << ")\n";				\
      }								\
      (log_file_stream).flush();				\
    }								\
  } while(0)

#define LOG_BH_DELETEMIN1_STATE(log_file_stream, is_fuzzer_mode)	\
  do{									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream) << "(deleteMin min = " << min			\
			  << ", max = " << max				\
			  << ", len = " << len				\
			  << ", isHeap = " << isHeap			\
			  << ", min1 = " << min1			\
			  << ", max1 = " << max1			\
			  << ", len1 = " << len1			\
			  << ", ret1 = " << ret1			\
			  << ", isHeap1 = " << isHeap1			\
			  << ")\n" ;					\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)

#define LOG_BH_DELETEMIN_STATE(log_file_stream, is_fuzzer_mode)	\
  do{								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(deleteMin len = " << len		\
			  << ", isHeap = " << isHeap		\
			  << ", len1 = " << len1		\
			  << ", ret1 = " << ret1		\
			  << ", isHeap1 = " << isHeap1		\
			  << ")\n" ;				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_BH_DOWNHEAP_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(downHeap isHeap2 = " << isHeap2	\
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


class BinaryHeap {
public:
  std::vector<int> heap;

  BinaryHeap() = default;
  
  BinaryHeap(std::vector<int> elements) {
    heap = elements;
    std::make_heap(heap.begin(), heap.end(), std::greater<int>());
  }

  void insert(int key) {
    heap.push_back(key);
    std::push_heap(heap.begin(), heap.end(), std::greater<int>());
  }

  int deleteMin() {
    if (heap.empty()) {
      return MAX; 
    }

    int min_element = heap.front();
    std::iter_swap(heap.begin(), heap.end() - 1);
    heap.pop_back();
    downHeap();

    return min_element;
  }

  void downHeap() {
    size_t size = heap.size();
    if (size <= 1) {
      return;
    }

    size_t parent = 0;
    size_t child;

    while ((child = 2 * parent + 2) < size) {
      if (heap[child] < heap[child - 1]) {
	child = child;
      } else {
	child = child - 1;
      }

      if (heap[parent] > heap[child]) {
	std::iter_swap(heap.begin() + parent, heap.begin() + child);
	parent = child;
      } else {
	return;
      }
    }

    child = 2 * parent + 1;
    if (child < size && heap[parent] > heap[child]) {
      std::iter_swap(heap.begin() + parent, heap.begin() + child);
    }
  }
    
  int len() const {
    return heap.size();
  }

  bool isEmpty() const {
    return heap.empty();
  }

  int isHeap() const {
    return static_cast<int>(is_heap(heap.begin(), heap.end(), std::greater<int>()));
  }

  int min() const {
    if (heap.empty()) {
      return MAX; // undefined behavior
    }
    return heap.front();
  }

  int max() const {
    if (heap.empty()) {
      return MIN;
    } 
    auto max_it = std::max_element(heap.begin(), heap.end());
    return *max_it;
  }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_DELETEMIN = 0x02;
constexpr uint8_t CMD_DOWNHEAP = 0x03;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init (BinaryHeap &bh, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen;i++) {
    uint8_t command = buf[i]%4;
    const uint8_t *valptr = &buf[i + 1];
    int v;
    
    switch (command) {
    case CMD_INSERT: {
      if ((i+1+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      bh.insert(v);
      i +=1;
      break;
    }

    case CMD_DELETEMIN: {
      int ret1 = bh.deleteMin();
      break;
    }

    case CMD_DOWNHEAP: {
      bh.downHeap();
      break;
    }

    default:
      break;
    }
  }
}
