#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <climits> // For INT_MAX and INT_MIN
#include <cstring> // For memset
#include <cassert> // For assert
#include <algorithm> // For std::min/max
#include <unistd.h>

#define MIN -129
#define MAX 128

#define DECLARE_FHM_INSERT_STATE_VARS()		\
  int32_t k, v;					\
  int len, containsk;				\
  int len1, containsk1;


#define DECLARE_FHM_REMOVEALL_STATE_VARS()	\
  int len, remove_count;			\
  int len1, remove_count1;


#define DECLARE_FHM_REMOVENONE_STATE_VARS()	\
  int len, remove_count;			\
  int len1, remove_count1;


#define DECLARE_FHM_ERASE_STATE_VARS()		\
  int k, flag;					\
  int len;					\
  int len1, ret1;


#define FHM_INSERT_WITH_STATE(fhm_obj, key_param, val_param)	\
  do{								\
    k = key_param;						\
    v = val_param;						\
    len = (fhm_obj).len();					\
    containsk = (fhm_obj).contains(k);				\
    (fhm_obj).insert(k, v);					\
    len1 = (fhm_obj).len();					\
    containsk1 = (fhm_obj).contains(k);				\
  }while(0)


#define FHM_REMOVEALL_WITH_STATE(fhm_obj)	\
  do{						\
    len = (fhm_obj).len();			\
    remove_count = 0;				\
    remove_count1 = (fhm_obj).len();		\
    (fhm_obj).remove_all();			\
    len1 = (fhm_obj).len();			\
  }while(0)


#define FHM_REMOVENONE_WITH_STATE(fhm_obj)	\
  do{						\
    len = (fhm_obj).len();			\
    remove_count = 0;				\
    len1 = (fhm_obj).len();			\
    remove_count1 = 0;				\
  }while(0)


#define FHM_ERASE_WITH_STATE(fhm_obj, key_param, flag_param)	\
  do{								\
    k = key_param;						\
    flag = flag_param;						\
    len = (fhm_obj).len();					\
    ret1 = (fhm_obj).erase(k, flag);				\
    len1 = (fhm_obj).len();					\
  }while(0)


#define LOG_FHM_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)  << "(insert k=" << k			\
			   << ", v=" << v			\
			   << ", len=" << len			\
			   << ", containsk=" << containsk	\
			   << ", len1=" << len1			\
			   << ", containsk1=" << containsk1	\
			   << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_FHM_REMOVEALL_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream)						\
	  << "(remove_all len=" << len					\
	  << ", remove_count=" << remove_count				\
	  << ", len1=" << len1						\
	  << ", remove_count1=" << remove_count1			\
	  << ")\n";							\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)


#define LOG_FHM_REMOVENONE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {									\
    if (!(log_file_stream)) {						\
      std::cerr << "Error: Unable to open log file." << "\n";		\
      exit(1);								\
    }									\
    if (!(is_fuzzer_mode)) {						\
      if ((log_file_stream).is_open()) {				\
	(log_file_stream)						\
	  << "(remove_none len=" << len					\
	  << ", remove_count=" << remove_count				\
	  << ", len1=" << len1						\
	  << ", remove_count1=" << remove_count1			\
	  << ")\n";							\
	(log_file_stream).flush();					\
      }									\
    }									\
  } while(0)


#define LOG_FHM_ERASE_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream)					\
	  << "(erase k=" << k					\
	  << ", len=" << len					\
	  << ", flag=" << flag					\
	  << ", len1=" << len1					\
	  << ", ret1=" << ret1					\
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

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_uint8(valptr_##target_var_name);		\
  } while(0)

class FlatHashMap{
private:
  // Represents the state of a single slot in our flat array
  enum class BucketState {
    EMPTY,    // Never used
    OCCUPIED, // Contains a live key
    DELETED   // Was used, but is now deleted
  };

  // The data structure for each slot
  struct Bucket {
    int key;
    int value;
    BucketState state;

    Bucket() : key(0), value(0), state(BucketState::EMPTY) {}
  };

  std::vector<Bucket> buckets_; // The flat array
  size_t size_;                 // Number of OCCUPIED elements
  size_t capacity_;             // The total size of buckets_
  std::hash<int> hasher_;     // Hashing function

  // --- Private Helper Functions ---

  /**
   * @brief Finds the correct bucket for a key (for insert, erase, or contains).
   * @param key The key to find.
   * @param find_empty If true, returns the first EMPTY or DELETED slot (for insertion).
   * If false, only finds an OCCUPIED slot (for search/erase).
   * @return The index of the bucket.
   */
  size_t find_bucket(int key, bool find_empty) {
    size_t index = hasher_(key) % capacity_;
    size_t start_index = index;

    while (true) {
      auto state = buckets_[index].state;

      if (find_empty) {
	// For insertion: stop at the first non-occupied slot
	if (state == BucketState::EMPTY || state == BucketState::DELETED) {
	  return index;
	}
      } else {
	// For search/erase: stop at an empty slot (key not found)
	if (state == BucketState::EMPTY) {
	  return -1; // Not found
	}
      }
            
      // If this slot is occupied, check if the key matches
      if (state == BucketState::OCCUPIED && buckets_[index].key == key) {
	return index; // Found matching key
      }

      // Move to the next slot (linear probing)
      index = (index + 1) % capacity_;

      // If we've looped all the way around, the map is full or key not found
      if (index == start_index) {
	return -1; // Full or not found
      }
    }
  }

  /**
   * @brief Doubles the size of the map and re-hashes all elements.
   */
  void resize() {
    // Save old data
    std::vector<Bucket> old_buckets = std::move(buckets_);
    size_t old_capacity = capacity_;

    // Create new, larger bucket array
    capacity_ *= 2;
    buckets_.clear();
    buckets_.resize(capacity_);
    size_ = 0; // Reset size, as insert will increment it

    // Re-hash all *occupied* elements from the old array
    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_buckets[i].state == BucketState::OCCUPIED) {
	// Use the public insert method to re-hash
	insert(old_buckets[i].key, old_buckets[i].value);
      }
    }
  }

public:
  /**
   * @brief Default constructor. Starts with a small initial capacity.
   */
  FlatHashMap() : size_(0), capacity_(8) {
    buckets_.resize(capacity_);
  }

  /**
   * @brief Inserts a key-value pair. Overwrites if key already exists.
   */
  void insert(int key, int value){
    // Check if we need to resize
    // (Load factor > 0.75)
    if (size_ > capacity_ * 0.75) {
      resize();
    }

    size_t index = find_bucket(key, true);

    if (index == -1) {
      // This should ideally not happen if we resize properly
      throw std::runtime_error("Hash map is full");
    }
        
    // If we are inserting into a brand new slot, increment size
    if (buckets_[index].state != BucketState::OCCUPIED) {
      size_++;
    }
        
    // Insert/overwrite data
    buckets_[index].key = key;
    buckets_[index].value = value;
    buckets_[index].state = BucketState::OCCUPIED;
  }

  /**
   * @brief Erases an element by key.
   * @return The key if an element was erased, otherwise MIN.
   */
  int erase(int key) {
    size_t index = find_bucket(key, false); // false = don't find empty

    if (index != -1 && buckets_[index].state == BucketState::OCCUPIED) {
      buckets_[index].state = BucketState::DELETED; // Mark as deleted
      size_--;
      return key;
    } else {
      return MIN; // Not found
    }
  }

  /**
   * @brief Erases an element by key, but only if flag == 1.
   * @return The key if an element was erased, otherwise MIN.
   */
  int erase(int key, int flag) {
    if (flag == 1) {
      return erase(key); // Call the main erase function
    }
    return MIN;
  }

  /**
   * @brief Checks if the map contains a given key.
   * @return 1 if the key is found, 0 otherwise.
   */
  int contains(int k){
    size_t index = find_bucket(k, false); // false = don't find empty
    return (index != -1 && buckets_[index].state == BucketState::OCCUPIED) ? 1 : 0;
  }

  /**
   * @brief Removes all elements from the map.
   */
  void remove_all(){
    // Reset all buckets to EMPTY
    buckets_.clear();
    buckets_.resize(capacity_);
    size_ = 0;
  }

  /**
   * @brief A no-op method that returns 0.
   */
  int remove_none(){
    return 0;
  }

    
  /**
   * @brief Returns the number of elements in the map.
   */
  int len(){
    return size_;
  }

  /**
   * @brief Finds the smallest key in the map by iterating all buckets.
   * @return The smallest key, or MIN if the map is empty.
   */
  int minKey(){
    if(size_ == 0){
      return MIN;
    }
        
    int min_val = INT_MAX;
    bool found = false;
        
    // Must iterate the *entire* array
    for(size_t i = 0; i < capacity_; ++i) {
      if (buckets_[i].state == BucketState::OCCUPIED) {
	if (!found || buckets_[i].key < min_val) {
	  min_val = buckets_[i].key;
	  found = true;
	}
      }
    }
    return found ? min_val : MIN;
  }

  /**
   * @brief Finds the largest key in the map by iterating all buckets.
   * @return The largest key, or MAX if the map is empty.
   */
  int maxKey(){
    if(size_ == 0){
      return MAX;
    }
        
    int max_val = INT_MIN;
    bool found = false;
        
    // Must iterate the *entire* array
    for(size_t i = 0; i < capacity_; ++i) {
      if (buckets_[i].state == BucketState::OCCUPIED) {
	if (!found || buckets_[i].key > max_val) {
	  max_val = buckets_[i].key;
	  found = true;
	}
      }
    }
    // Note: Your original returned MAX if empty.
    return found ? max_val : MAX; 
  }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_ERASE = 0x02;
constexpr uint8_t CMD_REMOVEALL = 0x03;
constexpr uint8_t CMD_REMOVENONE = 0x04;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = (uint8_t)b[0];
  return static_cast<uint32_t>(u);
}

void init (FlatHashMap &fhm, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen;i++) {
    uint8_t command = buf[i] % 5;
    int32_t k, v, flag;

    switch (command) {
    case CMD_INSERT: {
      if ((i+2+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      READ_INT8_FROM_FUZZBUF(buf, i+2, v);
      fhm.insert(k, v);
      i +=2;
      break;
    }
    case CMD_ERASE: {
      if ((i+2+1) > initLen) {
	i = initLen;
	break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+2, flag);
      READ_INT8_FROM_FUZZBUF(buf, i+1, k);
      flag = flag % 2;

      fhm.erase(k, flag);
      i +=2;
      break;
    }
    case CMD_REMOVEALL: {
      fhm.remove_all();
      break;
    }
    case CMD_REMOVENONE: {
      fhm.remove_none();
      break;
    }
    default:
      break;
    }
  }
}
