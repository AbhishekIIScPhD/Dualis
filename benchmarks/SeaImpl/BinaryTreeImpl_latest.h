#include <climits> // For INT_MAX
#include "seahorn/seahorn.h"
#include <vector>

using namespace std;

/* #define MIN -129
#define MAX 128
 */
#define MIN SHRT_MIN
#define MAX SHRT_MAX

struct Node {
    int key;
    Node *left;
    Node *right;
    
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node *root; 
    Node* insertHelper(Node* node, int key) {
      Node* newNode = new Node(key);
    
      // If the tree is empty, the new node becomes the root
      if (node == nullptr) {
        return newNode;
      }

    Node* current = node;
    Node* parent = nullptr;

    // Find the insertion point
    while (current != nullptr) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Attach to the parent's left or right
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    return node;
}

int searchHelper(Node* node, int key) const {
    Node* current = node;

    while (current != nullptr) {
        if (key == current->key) {
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
