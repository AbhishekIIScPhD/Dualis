#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <iostream>

class Set {
public:
    struct Node {
        int key;
        Node *left, *right;
        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }
        Node* curr = root;
        Node* prev = nullptr;
        while (curr) {
            prev = curr;
            if (key == curr->key) return;
            if (key < curr->key) {
	      curr = curr->left;
	    } else {
	      curr = curr->right;
	    }
        }

	if (key < prev->key) {
	    prev->left = new Node(key);
	} else {
	  prev->right = new Node(key);
	}
    }

    int remove() {
        if (!root) return 0;
        Node* temp = root;
        int val = temp->key;
        
        if (!root->left) root = root->right;
        else if (!root->right) root = root->left;
        else {
            Node* leftmost = root->left;
            while (leftmost->right) leftmost = leftmost->right;
            leftmost->right = root->right;
            root = root->left;
        }
        delete temp;
        return val;
    }

    bool empty() const {
      return root == nullptr;
    }
};

int main(int argc, char *argv[]) {
      bool fuzzer_mode = getenv("FUZZING") != nullptr;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <log_file>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::app);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file: " << filePath << std::endl;
    return 1;
  }

  while (__AFL_LOOP(10000)) {
    uint8_t buffer[4096];
    ssize_t bytes_read = read(0, buffer, sizeof(buffer));
    
    Set S;
    int16_t N_raw;
    std::memcpy(&N_raw, &buffer[0], 2);

    unsigned int N = (N_raw < 0 ? -N_raw : N_raw) % 1000;

    if (((bytes_read -2) < N) or N < 1000) 
      continue;

    size_t current_offset = 2;
      
    for (unsigned int i = 0; i < N; i++) {
      if (current_offset + 2 > (size_t) bytes_read) 
	      break;
      
      int16_t v1;
      std::memcpy(&v1, &buffer[current_offset], 2);
      current_offset += 2;
	
      if (v1 >= 0 || v1 == -4127) {
	      S.insert(v1);
      }
    }
      
    long long sum = 0;
    while (!S.empty()) {
      int v2 = S.remove();
      sum = sum + v2;
    }

    if (sum < 0) {
      ceFile << "Sum Result: " << sum << "\n";
      ceFile.flush();
    }
    assert(sum >= 0);
  }
  return 0;
}
