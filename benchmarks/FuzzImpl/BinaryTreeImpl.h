#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <filesystem>
#include <unistd.h>
#include <iomanip>
#include <cstdint>
#include <string>
#include <algorithm>
#include <limits.h>

using namespace std;

#define MIN -256
#define MAX 255
// #define MIN SHRT_MIN
// #define MAX SHRT_MAX


#define DECLARE_BT_INSERT_STATE_VARS()		\
  int n;					\
  int min, isEmpty;				\
  int min1, isEmpty1;

#define DECLARE_BT_SEARCH_STATE_VARS()		\
  int v;					\
  int min, isEmpty;				\
  int ret1;

#define BT_INSERT_WITH_STATE(bt_obj, key_param)	\
  do {						\
    min = (bt_obj).min();			\
    isEmpty = (bt_obj).isEmpty();		\
						\
    (bt_obj).insert(key_param);			\
						\
    min1 = (bt_obj).min();			\
    isEmpty1 = (bt_obj).isEmpty();		\
  } while(0)

#define BT_SEARCH_WITH_STATE(bt_obj, key_param)	\
  do {						\
    min = (bt_obj).min();			\
    isEmpty = (bt_obj).isEmpty();		\
						\
    ret1 = (bt_obj).search(key_param);		\
  } while(0)

#define LOG_BT_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert n = " << n		\
			  << ", min = " << min			\
			  << ", min1 = " << min1		\
			  << ", isEmpty = " << isEmpty		\
			  << ", isEmpty1 = " << isEmpty1	\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_BT_SEARCH_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if((log_file_stream).is_open()){				\
        (log_file_stream) << "(search v = " << v		\
			  << ", min = " << min			\
			  << ", isEmpty = " << isEmpty		\
			  << ", ret1 = " << ret1		\
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

#define READ_UINT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                                                   \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];     \
    target_var_name = read_uint8(valptr_##target_var_name);              \
  } while(0)

#define READ_INT16_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)	\
  do {									\
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];	\
    target_var_name = read_int16(valptr_##target_var_name);		\
  } while(0)

#define READ_UINT16_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                                                   \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];     \
    target_var_name = read_uint16(valptr_##target_var_name);              \
  } while(0)


struct Node {
    int key;
    Node *left;
    Node *right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node *root;
    Node* insertHelper(Node* root, int key) {
    Node* newNode = new Node(key);
    if (root == nullptr) {
        return newNode;
    }

    Node* parent = nullptr;
    Node* current = root;

    // Traverse down to find the appropriate null spot
    while (current != nullptr) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Attach the new node to the parent
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    return root;
  }

int searchHelper(Node* root, int key) const {
    Node* current = root;

    // Walk the tree until we hit a match or a leaf
    while (current != nullptr) {
        if (current->key == key) {
            return 1; // Found
        }

        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return 0; // Not found
}

    Node* minHelper(Node* node) const {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    BinaryTree(std::vector<short> elements) : root(nullptr) {
        for (auto e : elements) {
            insert(e);
        }
    }

    ~BinaryTree() {
        destroyTree(root);
    }

    void insert(int key) {
        root = insertHelper(root, key);
    }

    int search(int key) const {
        return searchHelper(root, key);
    }

    int min() const {
        if (root == nullptr) {
            return MAX;
        }
        Node* minNode = minHelper(root);
        return minNode->key;
    }

    int isEmpty() const {
        return root == nullptr ? 1 : 0;
    }
};



constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_SEARCH = 0x02;

static int32_t read_int8(const uint8_t *b) {
  int8_t u = (int8_t)b[0];
  return static_cast<int32_t>(u);
}

static uint32_t read_uint8(const uint8_t *b) {
  uint8_t u = b[0];
  return static_cast<uint32_t>(u);
}

static int32_t read_int16(const uint8_t *b) {
  int16_t u = (int16_t)((b[1] << 8) | b[0]);
  return static_cast<int32_t>(u);
}

static uint32_t read_uint16(const uint8_t *b) {
  uint16_t u = (uint16_t)((b[1] << 8) | b[0]);
  return static_cast<uint32_t>(u);
}

void init(BinaryTree &bt, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen;i++) {
    uint8_t command = buf[i]%3;
    int32_t v;

    switch (command) {
    case CMD_INSERT: {
      if ((i+1+1) > initLen) {
        i = initLen;
        break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      bt.insert(v);
      i +=1;
      break;
    }

    case CMD_SEARCH: {
      if ((i+1+1) > initLen) { 
        i = initLen;
        break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v); 
      bt.search(v);
      i +=1;
      break;
    }
    default:
      break;
    }
  }
}
