#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // For std::min_element, std::min
#include <climits>   // For INT_MAX
#include <cstdint>   // For fuzzer types
#include <unistd.h>    // For ssize_t
#include <map>       // For heap simulation
#include <set>       // For visited set
#include <cmath>     // For log2 in consolidate

// --- Fuzzer Configuration ---
#define MIN_N 2
#define MAX_N 10 // Max 10 nodes for manageable fuzzing
#define MAX_W 100 // Max edge weight
#define INF INT_MAX

// --- Generated Macros from Client Spec (Identical to previous) ---

#define DECLARE_DIJKSTRA_SPEC_VARS()		\
  int N, s, u, v;				\
  int u_extract_time, v_extract_time;		\
  int minu, minv;				\
  int visitedNodes;				\
  int x, xmin; /* Return values from extractMin */

#define DIJKSTRA_EXTRACTMIN_WITH_STATE(fh_obj, visitedNodes_param) \
  do {								\
    std::pair<int, int> result = (fh_obj).extractMin();		\
    x = result.first;						\
    xmin = result.second;					\
								\
    if (x == u && (fh_obj).u_extract_time > (fh_obj).N) {	\
      (fh_obj).u_extract_time = visitedNodes_param;		\
      (fh_obj).minu = xmin;					\
    } else if (x == v && (fh_obj).v_extract_time > (fh_obj).N) {	\
      (fh_obj).v_extract_time = visitedNodes_param;		\
      (fh_obj).minv = xmin;					\
    }								\
    /* Update public spec vars for logging */			\
    u_extract_time = (fh_obj).u_extract_time;			\
    v_extract_time = (fh_obj).v_extract_time;			\
    minu = (fh_obj).minu;					\
    minv = (fh_obj).minv;					\
  } while(0)

#define DIJKSTRA_UPDATEDISTANCE_WITH_STATE(fh_obj, x_param)	\
  do {								\
    (fh_obj).updateDistance(x_param);				\
  } while(0)

#define LOG_DIJKSTRA_EXTRACTMIN_STATE(log_file_stream, is_fuzzer_mode) \
  do { /* ... logging code ... */ } while(0) // (Omitted for brevity)

#define LOG_DIJKSTRA_UPDATEDISTANCE_STATE(log_file_stream, is_fuzzer_mode) \
  do { /* ... logging code ... */ } while(0) // (Omitted for brevity)


// --- Class Definition for Fuzzer ---

class FibonnacciHeap {
private:
  // --- Core Node Structure ---
  struct Node {
    int key;   // distance
    int value; // vertex ID
    int degree;
    bool mark;
    Node *parent;
    Node *child;
    Node *left;
    Node *right;
  };

  Node *minNode;
  int numNodes;
  std::vector<Node*> nodes; // Direct access to nodes by vertex ID

  // --- Internal Heap Operations ---

  // Creates a new node
  Node* createNode(int key, int value) {
    Node* n = new Node;
    n->key = key;
    n->value = value;
    n->degree = 0;
    n->mark = false;
    n->parent = nullptr;
    n->child = nullptr;
    n->left = n;
    n->right = n;
    return n;
  }

  // Inserts a node into the root list
  void fibHeapInsert(Node* node) {
    if (minNode == nullptr) {
      minNode = node;
    } else {
      node->right = minNode;
      node->left = minNode->left;
      minNode->left->right = node;
      minNode->left = node;
      if (node->key < minNode->key) {
	minNode = node;
      }
    }
    numNodes++;
  }

  // Removes a node from a doubly-linked list
  void removeFromList(Node* node) {
    node->left->right = node->right;
    node->right->left = node->left;
  }

  // Links heap y as a child of heap x
  void link(Node* y, Node* x) {
    removeFromList(y);
    y->parent = x;
    if (x->child == nullptr) {
      x->child = y;
      y->right = y;
n      y->left = y;
    } else {
      y->right = x->child;
      y->left = x->child->left;
      x->child->left->right = y;
      x->child->left = y;
    }
    x->degree++;
    y->mark = false;
  }

  // Consolidates the root list
  void consolidate() {
    if (minNode == nullptr) return;
    int maxDegree = static_cast<int>(std::floor(std::log2(numNodes))) + 2;
    std::vector<Node*> A(maxDegree, nullptr);

    std::vector<Node*> rootNodes;
    Node* current = minNode;
    do {
      rootNodes.push_back(current);
      current = current->right;
    } while (current != minNode);

    for (Node* x : rootNodes) {
      int d = x->degree;
      while (A[d] != nullptr) {
	Node* y = A[d];
	if (x->key > y->key) {
	  std::swap(x, y);
	}
	link(y, x);
	A[d] = nullptr;
	d++;
      }
      A[d] = x;
    }

    minNode = nullptr;
    for (Node* node : A) {
      if (node != nullptr) {
	if (minNode == nullptr) {
	  minNode = node;
	  node->left = node;
	  node->right = node;
	} else {
	  node->right = minNode;
	  node->left = minNode->left;
	  minNode->left->right = node;
	  minNode->left = node;
	  if (node->key < minNode->key) {
	    minNode = node;
	  }
	}
      }
    }
  }

  // Core extract-min operation
  Node* fibHeapExtractMin() {
    Node* z = minNode;
    if (z != nullptr) {
      if (z->child != nullptr) {
	Node* child = z->child;
	do {
	  Node* nextChild = child->right;
	  child->right = minNode;
	  child->left = minNode->left;
	  minNode->left->right = child;
	  minNode->left = child;
	  child->parent = nullptr;
	  child = nextChild;
	} while (child != z->child);
      }
      
      removeFromList(z);

      if (z == z->right) {
	minNode = nullptr;
      } else {
	minNode = z->right;
	consolidate();
      }
      numNodes--;
    }
    return z;
  }

  // Cuts a node from its parent
  void cut(Node* node, Node* parent) {
    removeFromList(node);
    if (node == node->right) {
      parent->child = nullptr;
    } else {
      parent->child = node->right;
    }
    parent->degree--;
    
    // Add node to root list
    node->right = minNode;
    node->left = minNode->left;
    minNode->left->right = node;
    minNode->left = node;
    
    node->parent = nullptr;
    node->mark = false;
  }

  // Performs cascading cut
  void cascadingCut(Node* node) {
    Node* p = node->parent;
    if (p != nullptr) {
      if (!node->mark) {
	node->mark = true;
      } else {
	cut(node, p);
	cascadingCut(p);
      }
    }
  }

  // Core decrease-key operation
  void fibHeapDecreaseKey(Node* node, int newKey) {
    if (newKey > node->key) {
      return; // Error
    }
    node->key = newKey;
    Node* p = node->parent;
    if (p != nullptr && node->key < p->key) {
      cut(node, p);
      cascadingCut(p);
    }
    if (node->key < minNode->key) {
      minNode = node;
    }
  }

public:
  // --- Public State (for Fuzzer) ---
  int N, s, u, v;
  std::vector<std::vector<int>> G;
  std::set<int> visited;
  int u_extract_time, v_extract_time, minu, minv;

  // --- Constructor ---
  FibonnacciHeap() : minNode(nullptr), numNodes(0), N(0), s(0), u(0), v(0) {
    resetSpecVars();
  }

  // --- Destructor ---
  ~FibonnacciHeap() {
    for (Node* n : nodes) {
      delete n;
    }
  }

  void resetSpecVars() {
    u_extract_time = N + 1;
    v_extract_time = N + 1;
    minu = INF;
    minv = INF;
  }

  // --- Fuzzer Interface Methods ---

  // Corresponds to fh.initialize(N, s, visitedNodes);
  void initialize(int n_val, int s_val, int u_val, int v_val, 
                  const std::vector<std::vector<int>>& g_val) {
    // Clear old state
    for (Node* n : nodes) {
      delete n;
    }
    nodes.clear();
    minNode = nullptr;
    numNodes = 0;
    visited.clear();

    // Set new state
    N = n_val;
    s = s_val;
    u = u_val;
    v = v_val;
    G = g_val;
    resetSpecVars();
    nodes.resize(N);

    // Create and insert all nodes for the graph
    for (int i = 0; i < N; ++i) {
      int key = (i == s) ? 0 : INF;
      Node* node = createNode(key, i);
      nodes[i] = node;
      fibHeapInsert(node);
    }
  }

  // Corresponds to fh.extractMin(visitedNodes)
  std::pair<int, int> extractMin() {
    Node* min = fibHeapExtractMin();
    if (min == nullptr) {
      return {-1, INF};
    }
    visited.insert(min->value);
    return {min->value, min->key};
  }

  // Corresponds to fh.updateDistance(x)
  void updateDistance(int x) {
    if (x < 0 || x >= N) return; // Invalid node
    
    Node* xNode = nodes[x];
    if (xNode->key == INF) return; // Unreachable
    int xDist = xNode->key;

    // Relax neighbors
    for (int neighbor = 0; neighbor < N; ++neighbor) {
      if (G[x][neighbor] > 0 && // There is an edge
	  visited.find(neighbor) == visited.end()) { // Not yet visited
	
	Node* neighborNode = nodes[neighbor];
	int new_dist = xDist + G[x][neighbor];
	
	if (new_dist < neighborNode->key) {
	  // This is the "decrease-key" operation
	  fibHeapDecreaseKey(neighborNode, new_dist);
	}
      }
    }
  }
};

// --- Fuzzer `init` Function (Identical to previous) ---

void init(FibonnacciHeap &fh, std::vector<uint8_t>& buf) {
  if (buf.size() < 3 + (MAX_N * MAX_N)) {
    return; // Not enough data
  }
  
  int N = (buf[0] % (MAX_N - MIN_N + 1)) + MIN_N;
  int s = buf[1] % N;
  int u = buf[2] % N;
  int v = buf[3] % N;
  
  std::vector<std::vector<int>> G(N, std::vector<int>(N, 0));
  
  size_t buf_idx = 4;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j) {
	G[i][j] = 0;
      } else {
	if (buf_idx >= buf.size()) {
	  i = N; break;
	}
	int weight = (int)buf[buf_idx++] % MAX_W; 
	G[i][j] = weight;
      }
    }
  }

  fh.initialize(N, s, u, v, G);
}
