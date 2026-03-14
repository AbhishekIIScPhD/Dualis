#include <cassert>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits> 
#include <filesystem>
#include <algorithm>
#include <set>      
#include <cstdint>  
#include <string>   

using namespace std;

#define MIN -129
#define MAX 128

#define DECLARE_RBT_INSERT_STATE_VARS()		\
  int i;					\
  int max, min, len, noDup;			\
  int max1, min1, len1, noDup1;

#define DECLARE_RBT_SEARCH_STATE_VARS()		\
  int data;					\
  int max, min, len, noDup;			\
  int ret1;

#define RBT_INSERT_WITH_STATE(rbt_obj, key_param)	\
  do {							\
    max = (rbt_obj).max();				\
    min = (rbt_obj).min();				\
    len = (rbt_obj).len();				\
    noDup = (rbt_obj).noDup();				\
							\
    (rbt_obj).insert(key_param);			\
							\
    max1 = (rbt_obj).max();				\
    min1 = (rbt_obj).min();				\
    len1 = (rbt_obj).len();				\
    noDup1 = (rbt_obj).noDup();				\
  } while(0)

#define RBT_SEARCH_WITH_STATE(rbt_obj, key_param)	\
  do {							\
    max = (rbt_obj).max();				\
    min = (rbt_obj).min();				\
    len = (rbt_obj).len();				\
    noDup = (rbt_obj).noDup();				\
							\
    ret1 = (rbt_obj).search(key_param);			\
  } while(0)

#define LOG_RBT_INSERT_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
	(log_file_stream) << "(insert i = " << i			\
			  << ", max = " << max			\
			  << ", max1 = " << max1		\
			  << ", min = " << min			\
			  << ", min1 = " << min1		\
			  << ", len = " << len			\
			  << ", len1 = " << len1		\
			  << ", noDup = " << noDup		\
			  << ", noDup1 = " << noDup1		\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define LOG_RBT_SEARCH_STATE(log_file_stream, is_fuzzer_mode)	\
  do {								\
    if (!(log_file_stream)) {					\
      std::cerr << "Error: Unable to open log file." << "\n";	\
      exit(1);							\
    }								\
    if (!(is_fuzzer_mode)) {					\
      if ((log_file_stream).is_open()) {			\
        (log_file_stream) << "(search data = " << data		\
			  << ", max = " << max			\
			  << ", min = " << min			\
			  << ", len = " << len			\
			  << ", noDup = " << noDup		\
			  << ", ret1 = " << ret1		\
			  << ")\n";				\
	(log_file_stream).flush();				\
      }								\
    }								\
  } while(0)

#define READ_INT8_FROM_FUZZBUF(buffer_ptr, offset, target_var_name)     \
  do {                                      \
    const uint8_t *valptr_##target_var_name = &(buffer_ptr)[offset];    \
    target_var_name = read_int8(valptr_##target_var_name);          \
  } while(0)


enum Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    Node *parent;
    Node *left;
    Node *right;
    Node(int k) : key(k), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree {
private:
    Node *root;
    Node *NIL;
    int nodeCount; 

    void leftRotate(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NIL) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *y) {
        Node *x = y->left;
        y->left = x->right;
        if (x->right != NIL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == NIL) {
            this->root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertFixup(Node *z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node *y = z->parent->parent->right; 
                if (y->color == RED) { 
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) { 
                        z = z->parent;
                        leftRotate(z);
                    }
                    
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node *y = z->parent->parent->left; 
                if (y->color == RED) { 
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) { 
                        z = z->parent;
                        rightRotate(z);
                    }
                    
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    Node* searchHelper(Node* node, int key) const {
        if (node == NIL || key == node->key) {
            return node;
        }
        if (key < node->key) {
            return searchHelper(node->left, key);
        }
        return searchHelper(node->right, key);
    }

    Node* minHelper(Node* node) const {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    Node* maxHelper(Node* node) const {
        while (node->right != NIL) {
            node = node->right;
        }
        return node;
    }

    void destroyTree(Node* node) {
        if (node != NIL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
    /**
     * @brief Helper for noDup(). Fills a set with all keys.
     */
    void fillSet(Node* node, std::set<int>& s) {
        if (node == NIL) {
            return;
        }
        s.insert(node->key);
        fillSet(node->left, s);
        fillSet(node->right, s);
    }

public:
    RedBlackTree() : nodeCount(0) {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->left = nullptr;
        NIL->right = nullptr;
        root = NIL;
    }

    ~RedBlackTree() {
        destroyTree(this->root);
        delete NIL;
    }

    RedBlackTree(std::vector<int> elements) : RedBlackTree() {
        for (auto e : elements) {
            insert(e);
        }
    }
     
    void insert(int key) {
        Node *z = new Node(key);
        z->left = NIL;
        z->right = NIL;

        Node *y = NIL;
        Node *x = this->root;

        while (x != NIL) {
            y = x;
            
            if (z->key < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == NIL) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        nodeCount++;
        insertFixup(z);
    }

    bool search(int key) const {
        Node* result = searchHelper(this->root, key);
        return (result != NIL);
    }

    int noDup(){
        
        
        if (isEmpty()) {
            return 1;
        }
        std::set<int> s;
        fillSet(this->root, s);
        
        return s.size() == this->nodeCount ? 1 : 0;
    }

    int min() const {
        if (isEmpty()) {
            return MAX;
        }
        return minHelper(root)->key;
    }

    int max() const {
        if (isEmpty()) {
            return MIN - 1;
        }
        return maxHelper(root)->key;
    }

    int len() const {
        return nodeCount;
    }

    bool isEmpty() const {
        return root == NIL;
    }
};

constexpr uint8_t CMD_INSERT = 0x01;
constexpr uint8_t CMD_SEARCH = 0x02;

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

void init(RedBlackTree &rbt, std::vector<uint8_t>& buf, ssize_t initLen){
  for (ssize_t i = 0; i < initLen; i++) {
    uint8_t command = buf[i] % 3;
    int v; 

    switch (command) {
    case CMD_INSERT: {
      if ((i+1+1) > initLen) {
        i = initLen;
        break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      rbt.insert(v);
      i += 1;
      break;
    }
    case CMD_SEARCH: {
      if ((i+1+1) > initLen) {
        i = initLen;
        break;
      }
      READ_INT8_FROM_FUZZBUF(buf, i+1, v);
      rbt.search(v);
      i += 1;
      break;
    }
    default:
      break;
    }
  }
}
