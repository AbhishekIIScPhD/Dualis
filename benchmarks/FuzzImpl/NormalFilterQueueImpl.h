#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h> 
#include <filesystem>
#include <algorithm>
#include <cassert>
#include <utility>
#include <unistd.h>


#define MAX -1
#define MIN -1

using namespace std;
#define DECLARE_NFQ_APPEND_STATE_VARS()		\
  int32_t prio, packetSize;			\
  int len, maxPrio, minPacketSize;		\
  int len1, maxPrio1, minPacketSize1;

#define DECLARE_NFQ_PROCESSQUEUE_STATE_VARS()	\
  int len, maxPrio, minPacketSize;		\
  int len1, maxPrio1, minPacketSize1;

#define NFQ_APPEND_WITH_STATE(nfq_obj, prio_param, packetSize_param)	\
  do {									\
    minPacketSize = (nfq_obj).minPacketSize();				\
    maxPrio = (nfq_obj).maxPrio();					\
    len = (nfq_obj).len();						\
    prio = prio_param;							\
    packetSize = packetSize_param;					\
    (nfq_obj).append(make_pair(prio, packetSize));			\
    minPacketSize1 = (nfq_obj).minPacketSize();				\
    maxPrio1 = (nfq_obj).maxPrio();					\
    len1 = (nfq_obj).len();						\
  } while(0)


#define NFQ_PROCESSQUEUE_WITH_STATE(nfq_obj)	\
  do {						\
    len = (nfq_obj).len();			\
    minPacketSize = (nfq_obj).minPacketSize();	\
    maxPrio = (nfq_obj).maxPrio();		\
    (nfq_obj).processQueue();			\
    len1 = (nfq_obj).len();			\
    minPacketSize1 = (nfq_obj).minPacketSize();	\
    maxPrio1 = (nfq_obj).maxPrio();		\
  } while(0)


#define LOG_NFQ_APPEND_STATE(log_file_stream, is_fuzzer_mode)	\
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
	  << ", maxPrio = " << maxPrio				\
	  << ", minPacketSize = " << minPacketSize		\
	  << ", len1 = " << len1				\
	  << ", maxPrio1 = " << maxPrio1			\
	  << ", minPacketSize1 = "<< minPacketSize1		\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)


#define LOG_NFQ_PROCESSQUEUE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream)						\
	  << "(processQueue len = " << len				\
	  << ", maxPrio = " << maxPrio					\
	  << ", minPacketSize = " << minPacketSize			\
	  << ", len1 = " << len1					\
	  << ", maxPrio1 = " << maxPrio1				\
	  << ", minPacketSize1 = " << minPacketSize1			\
	  << ")\n";							\
	(log_file_stream).flush();					\
      }									\
    }									\
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

#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_int8(valptr_##target_var_name);		\
  } while(0)

class NormalFilterQueue {
private:
  vector<pair<int, int>> Q;  

public:
  NormalFilterQueue() = default;
  NormalFilterQueue(vector<pair<int, int>> elements) {
    for (auto e : elements) {
      append(e);
    }
  }

  void append(pair<int, int> packet) {
    Q.push_back(packet);  
  }

  void processQueue() {
    for (auto it = Q.begin(); it != Q.end(); ) {
      if (std::get<0>(*it) == 0 && std::get<1>(*it) >= 500) {
	it = Q.erase(it);
      } else {
	++it;
      }
    }
  }

  int len()  {
    return Q.size();
  }

  int maxPrio()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MAX;
    }
    auto result = *max_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
      return get<0>(lhs) < get<0>(rhs);
    });
    return get<0>(result);
  }

  int minPacketSize()  {
    if (Q.empty()) {
      cerr << "queue is empty!" << endl;
      return MIN;
    }
    auto result = *min_element(Q.cbegin(), Q.cend(), [](const auto& lhs, const auto& rhs) {
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

void init (NormalFilterQueue &nfq, std::vector<uint8_t>& buf, ssize_t initLen){
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
      nfq.append(std::make_pair(prio, pack));
      i += 2;
      break;
    }
    case CMD_PROCESSQUEUE: {
      nfq.processQueue();
      break;
    }
    default:
      break;
    }
  }
}
