#include <cassert>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> // for INT_MAX
#include <filesystem>
#include <algorithm>

#define MAX -1  // packetsize
#define MIN -1 // prio

using namespace std;

#define DECLARE_PFQ_APPEND_STATE_VARS()		\
  int32_t prio, packetSize;			\
  int len, minPrio, maxPacketSize;		\
  int len1, minPrio1, maxPacketSize1;

#define DECLARE_PFQ_PROCESSQUEUE_STATE_VARS()	\
  int len, minPrio, maxPacketSize;		\
  int len1, minPrio1, maxPacketSize1;

#define PFQ_APPEND_WITH_STATE(pfq_obj, prio_param, packetSize_param)	\
  do {									\
    maxPacketSize = (pfq_obj).maxPacketSize();				\
    minPrio = (pfq_obj).minPrio();					\
    len = (pfq_obj).len();						\
    prio = prio_param;							\
    packetSize = packetSize_param;					\
    (pfq_obj).append(make_pair(prio, packetSize));			\
    maxPacketSize1 = (pfq_obj).maxPacketSize();				\
    minPrio1 = (pfq_obj).minPrio();					\
    len1 = (pfq_obj).len();						\
  } while(0)


#define PFQ_PROCESSQUEUE_WITH_STATE(pfq_obj)	\
  do {						\
    len = (pfq_obj).len();			\
    maxPacketSize = (pfq_obj).maxPacketSize();	\
    minPrio = (pfq_obj).minPrio();		\
    (pfq_obj).processQueue();			\
    len1 = (pfq_obj).len();			\
    maxPacketSize1 = (pfq_obj).maxPacketSize();	\
    minPrio1 = (pfq_obj).minPrio();		\
  } while(0)


#define LOG_PFQ_APPEND_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(append prio = " << prio				\
	  << ", packetSize = " << packetSize			\
	  << ", len = " << len					\
	  << ", minPrio = " << minPrio				\
	  << ", maxPacketSize = " << maxPacketSize		\
	  << ", len1 = " << len1				\
	  << ", minPrio1 = " << minPrio1			\
	  << ", maxPacketSize1 = "<< maxPacketSize1		\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)


#define LOG_PFQ_PROCESSQUEUE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream)						\
	  << "(processQueue len = " << len				\
	  << ", minPrio = " << minPrio					\
	  << ", maxPacketSize = " << maxPacketSize			\
	  << ", len1 = " << len1					\
	  << ", minPrio1 = " << minPrio1				\
	  << ", maxPacketSize1 = " << maxPacketSize1			\
	  << ")\n";							\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)


#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_int8(valptr_##target_var_name);		\
  } while(0)

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_uint8(valptr_##target_var_name);		\
  } while(0)

class PriorityFilterQueue {
private:
  vector<pair<int, int>> Q;  // Vector to hold the queue elements

public:
  PriorityFilterQueue() = default;
  PriorityFilterQueue(vector<pair<int, int>> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(pair<int, int> packet) {
      Q.push_back(packet);
  }

  void processQueue() {
    for (auto it = Q.begin(); it != Q.end(); ) {
      if (std::get<0>(*it) == 1 && std::get<1>(*it) < 500) {
	it = Q.erase(it);
      } else {
	++it;
      }
    }
  }

  int len()  {
    return Q.size();
  }

  int minPrio()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MIN;
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int maxPacketSize()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MAX;
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<1>(lhs) < get<1>(rhs);
    });
    return get<1>(result);
  }

  bool isEmpty() { return Q.size() == 0; }
};

constexpr uint8_t CMD_APPEND = 0x01;
constexpr uint8_t CMD_PROCESSQUEUE = 0x02;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

void init (PriorityFilterQueue &pfq, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen;i++) {
    uint8_t command = buf[i] % 3;
    int prio, pack;

    switch (command) {
    case CMD_APPEND: {
      if ((i+2+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, prio);
      READ_INT8_FROM_FUZZBUF(buf, i+2, pack);
      pfq.append(std::make_pair(prio, pack));
      i += 2;
      break;
    }
    case CMD_PROCESSQUEUE: {
      pfq.processQueue();
      break;
    }
    default:
      break;
    }
  }
}
