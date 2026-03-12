#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <climits> 
#include <cstring> 
#include <cassert> 
#include <algorithm> 
#include <unistd.h>

#define MIN -129
#define MAX 128

#define DECLARE_FHS_INSERT_STATE_VARS()		\
  int32_t v;					\
  int len, min, max, containsv;			\
  int len1, min1, max1, containsv1;


#define DECLARE_FHS_RESERVE_STATE_VARS()	\
  int N;					\
  int len;					\
  int len1;

#define DECLARE_FHS_ERASE_STATE_VARS()		\
  int v, flag;					\
  int len, containsv, min, max;			\
  int len1, containsv1, min1, max1;

#define FHS_INSERT_WITH_STATE(fhs_obj, val_param)	\
  do{							\
    v = val_param;					\
    len = (fhs_obj).len();				\
    min = (fhs_obj).minKey();				\
    max = (fhs_obj).maxKey();				\
    containsv = (fhs_obj).contains(v);			\
    (fhs_obj).insert(v);				\
    len1 = (fhs_obj).len();				\
    min1 = (fhs_obj).minKey();				\
    max1 = (fhs_obj).maxKey();				\
    containsv1 = (fhs_obj).contains(v);			\
  }while(0)


#define FHS_RESERVE_WITH_STATE(fhs_obj, N_param)	\
  do{							\
    N = N_param;					\
    len = (fhs_obj).len();				\
    (fhs_obj).reserve(N);				\
    len1 = (fhs_obj).len();				\
  }while(0)


#define FHS_ERASE_WITH_STATE(fhs_obj, value_param)	\
  do{							\
    v = value_param;					\
    len = (fhs_obj).len();				\
    containsv = (fhs_obj).contains(v);			\
    min = (fhs_obj).minKey();				\
    max = (fhs_obj).maxKey();				\
    (fhs_obj).erase(v);					\
    len1 = (fhs_obj).len();				\
    containsv1 = (fhs_obj).contains(v);			\
    min1 = (fhs_obj).minKey();				\
    max1 = (fhs_obj).maxKey();				\
  }while(0)


#define LOG_FHS_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(insert v=" << v					\
	  << ", len=" << len					\
	  << ", containsv=" << containsv			\
	  << ", min=" << min					\
	  << ", max=" << max					\
	  << ", len1=" << len1					\
	  << ", containsv1=" << containsv1			\
	  << ", min1=" << min1					\
	  << ", max1=" << max1					\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_FHS_INSERT1_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(insert1 v=" << v					\
	  << ", len=" << len					\
	  << ", containsv=" << containsv			\
	  << ", min=" << min					\
	  << ", max=" << max					\
	  << ", len1=" << len1					\
	  << ", containsv1=" << containsv1			\
	  << ", min1=" << min1					\
	  << ", max1=" << max1					\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_FHS_ERASE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(erase v=" << v					\
	  << ", len=" << len					\
	  << ", containsv=" << containsv			\
	  << ", min=" << min					\
	  << ", max=" << max					\
	  << ", len1=" << len1					\
	  << ", containsv1=" << containsv1			\
	  << ", min1=" << min1					\
	  << ", max1=" << max1					\
	  << ")\n";						\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_FHS_RESERVE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(reserve len=" << len				\
	  << ", N=" << N					\
	  << ", len1=" << len1					\
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


class FlatHashSet{
private:
    
  enum class BucketState {
    EMPTY,    
    OCCUPIED, 
    DELETED   
  };

    
  struct Bucket {
    int key;
    BucketState state;

    Bucket() : key(0), state(BucketState::EMPTY) {}
  };

  std::vector<Bucket> buckets_; 
  size_t size_;                 
  size_t capacity_;             
  std::hash<int> hasher_;     
    
    
  static constexpr double MAX_LOAD_FACTOR = 0.75;

    

  /**
   * @brief Finds the correct bucket for a key.
   * @param key The key to find.
   * @return The index of the bucket. Returns (size_t)-1 if not found.
   */
  size_t find_bucket(int key) {
    size_t index = hasher_(key) % capacity_;
    size_t start_index = index;

    while (true) {
      auto state = buckets_[index].state;
      if (state == BucketState::EMPTY) {
	return -1; 
      }
      if (state == BucketState::OCCUPIED && buckets_[index].key == key) {
	return index; 
      }
      index = (index + 1) % capacity_;
      if (index == start_index) {
	return -1; 
      }
    }
  }

  /**
   * @brief Finds the first available slot for insertion.
   * @param key The key to insert.
   * @return The index of the bucket to insert into.
   */
  size_t find_bucket_for_insert(int key) {
    size_t index = hasher_(key) % capacity_;
    size_t start_index = index;
    while (true) {
      auto state = buckets_[index].state;
      if (state == BucketState::EMPTY || state == BucketState::DELETED) {
	return index;
      }
      if (state == BucketState::OCCUPIED && buckets_[index].key == key) {
	return index; 
      }
      index = (index + 1) % capacity_;
      if (index == start_index) {
	throw std::runtime_error("Hash set is full and did not resize.");
      }
    }
  }


  /**
   * @brief Doubles the size of the map and re-hashes all elements.
   */
  void resize(size_t new_capacity) {
    std::vector<Bucket> old_buckets = std::move(buckets_);
    size_t old_capacity = capacity_;
    capacity_ = new_capacity;
    buckets_.clear();
    buckets_.resize(capacity_);
    size_ = 0; 
    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_buckets[i].state == BucketState::OCCUPIED) {
                
	insert(old_buckets[i].key);
      }
    }
  }

public:
  /**
   * @brief Default constructor. Starts with a small initial capacity.
   */
  FlatHashSet() : size_(0), capacity_(8) {
    buckets_.resize(capacity_);
  }

  /**
   * @brief Inserts a key into the set.
   */
  void insert(int key){
    if (size_ > capacity_ * MAX_LOAD_FACTOR) {
      resize(capacity_ * 2);
    }

    size_t index = find_bucket_for_insert(key);
    if (buckets_[index].state != BucketState::OCCUPIED) {
      size_++;
    }
    buckets_[index].key = key;
    buckets_[index].state = BucketState::OCCUPIED;
  }

  /**
   * @brief (Duplicate) Inserts a key into the set.
   */
  void insert1(int key){
    insert(key);
  }

  /**
   * @brief Erases an element by key.
   */
  void erase(int key){
    size_t index = find_bucket(key);

    if (index != (size_t)-1 && buckets_[index].state == BucketState::OCCUPIED) {
      buckets_[index].state = BucketState::DELETED; 
      size_--;
    }
  }

  /**
   * @brief Checks if the set contains a given key.
   * @return 1 if the key is found, 0 otherwise.
   */
  int contains(int k){
    size_t index = find_bucket(k);
    return (index != (size_t)-1 && buckets_[index].state == BucketState::OCCUPIED) ? 1 : 0;
  }

  /**
   * @brief Ensures the set can hold at least 'size' elements without resizing.
   */
  void reserve(int size){
    if (size < 0) {
      return;
    }
        
    size_t required_capacity = (size_t)(size / MAX_LOAD_FACTOR) + 1;
    if (required_capacity > capacity_) {
      size_t new_capacity = capacity_;
      while (new_capacity < required_capacity) {
	new_capacity *= 2;
      }
      resize(new_capacity);
    }
  }

  /**
   * @brief Returns the number of elements in the set.
   */
  int len(){
    return size_;
  }

  /**
   * @brief Finds the smallest key in the set by iterating all buckets.
   * @return The smallest key, or MAX if the set is empty.
   */
  int minKey(){
    if(size_ == 0){
      return MAX; 
    }
        
    int min_val = INT_MAX;
    bool found = false;
        
        
    for(size_t i = 0; i < capacity_; ++i) {
      if (buckets_[i].state == BucketState::OCCUPIED) {
	if (!found || buckets_[i].key < min_val) {
	  min_val = buckets_[i].key;
	  found = true;
	}
      }
    }
    return found ? min_val : MAX; 
  }

  /**
   * @brief Finds the largest key in the set by iterating all buckets.
   * @return The largest key, or MIN if the set is empty.
   */
  int maxKey(){
    if(size_ == 0){
      return MIN; 
    }
        
    int max_val = INT_MIN;
    bool found = false;
    for(size_t i = 0; i < capacity_; ++i) {
      if (buckets_[i].state == BucketState::OCCUPIED) {
	if (!found || buckets_[i].key > max_val) {
	  max_val = buckets_[i].key;
	  found = true;
	}
      }
    }
    return found ? max_val : MIN; 
  }
};


constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_ERASE = 0x02;
constexpr uint8_t CMD_RESERVE = 0x03;

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

void init (FlatHashSet &fhs, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen;i++) {
    uint8_t command = buf[i] % 4;
    int32_t v, capacity;

    switch (command) {
    case CMD_INSERT: {
      if ((i+1+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      fhs.insert(v);
      i +=1;
      break;
    }
    case CMD_ERASE: {
      if ((i+1+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      fhs.erase(v);
      i +=1;
      break;
    }
    case CMD_RESERVE: {
      READ_INT8_FROM_FUZZBUF(buf, i+1, capacity);
      fhs.reserve(capacity);
      i+=1;
      break;
    }
    default:
      break;
    }
  }
}
