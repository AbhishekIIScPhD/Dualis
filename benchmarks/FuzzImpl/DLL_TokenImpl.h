#include <cassert>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits> 
#include <filesystem>
#include <stack>
#include <algorithm> 
#include <cstdint>   
#include <string>    

#define MIN -129
#define MAX 128

using namespace std;

#define DECLARE_DLT_PUSH_STATE_VARS()		\
  int val;					\
  int min, max, len;				\
  int min1, max1, len1;

#define DLT_PUSH_WITH_STATE(dlt_obj, val_param)	\
  do {						\
    min = (dlt_obj).min();			\
    max = (dlt_obj).max();			\
    len = (dlt_obj).len();			\
						\
    (dlt_obj).push(val_param);			\
						\
    min1 = (dlt_obj).min();			\
    max1 = (dlt_obj).max();			\
    len1 = (dlt_obj).len();			\
  } while(0)

#define LOG_DLT_PUSH_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
        (log_file_stream) << "(push val ="<< val		\
			  << ", min =" << min			\
			  << ", len = " << len			\
			  << ", max =" << max			\
			  << ", min1 =" << min1			\
			  << ", len1 = " << len1		\
			  << ", max1 =" << max1			\
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

struct Node {
  int key;
  Node* next;
  Node* prev;

  Node(int k) : key(k), next(nullptr), prev(nullptr) {}
};

class DLLTok {
private:
  Node* head;
  Node* tail;
  int count;

  void clear() {
    Node* current = head;
    while (current != nullptr) {
      Node* next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
  }

public:
  DLLTok() : head(nullptr), tail(nullptr), count(0) {}
  
  DLLTok(std::vector<int> elements) : head(nullptr), tail(nullptr), count(0) {
    for (auto e : elements) {
      push(e);
    }
  }

  ~DLLTok() {
    clear();
  }

  void push(int key) {
    Node* newNode = new Node(key);
    if (tail == nullptr) {
      
      head = newNode;
      tail = newNode;
    } else {
      
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    count++;
  }

  
  int max() const {
    if (head == nullptr) { 
      return MIN; 
    }

    int maxVal = head->key;
    Node* current = head->next;
    while (current != nullptr) {
      if (current->key > maxVal) {
	maxVal = current->key;
      }
      current = current->next;
    }
    return maxVal;
  }

  
  int min() const {
    if (head == nullptr) { 
      return MAX; 
    }

    int minVal = head->key;
    Node* current = head->next;
    while (current != nullptr) {
      if (current->key < minVal) {
	minVal = current->key;
      }
      current = current->next;
    }
    return minVal;
  }

  
  int len() const{
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

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                                                   \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];     \
    target_var_name = read_uint8(valptr_##target_var_name);              \
  } while(0)

void init(DLLTok &dlt, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen; i++) {
    uint8_t command = buf[i] % 2; 
    int v; 

    switch (command) {
    case CMD_PUSH: {
      if ((i+1+1) > initLen) { 
        i = initLen;
        break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      dlt.push(v);
      i += 1; 
      break;
    }
    default:
      break;
    }
  }
}
