#include <climits> // For INT_MAX
#include "seahorn/seahorn.h"

// Define a bound for the verification.
// SeaHorn will prove the property *up to this size*.
#define MAX_N 10

class BinaryTree {
private:
    int tree[MAX_N];
    int size;

public:
    // Constructor to initialize the size
    BinaryTree() : size(0) {}

    bool isEmpty() const {
        return size == 0;
    }

    // Your new, verifier-friendly insert()
    void insert(int key) {
        // --- Replaced std::vector::push_back ---
        // Check if we are within the bounded size
        if (size >= MAX_N) {
            return; // Can't insert anymore
        }
        tree[size] = key;
        int currentIndex = size;
        size++;
        // --- End of push_back replacement ---

        while (currentIndex > 0) {
            int parentIndex = (currentIndex - 1) / 2;
            if (tree[currentIndex] < tree[parentIndex]) {
                
                // --- Replaced std::swap ---
                int temp = tree[currentIndex];
                tree[currentIndex] = tree[parentIndex];
                tree[parentIndex] = temp;
                // --- End of swap replacement ---

                currentIndex = parentIndex;
            } else {
                break;
            }
        }
    }

    // Your new, verifier-friendly min()
    int min() const {
        if (isEmpty()) { // Use our new isEmpty
            return INT_MAX;
        }
        return tree[0]; 
    }

    // Your new, verifier-friendly search()
    int search(int key) const {
        for (int i = 0; i < size; i++) { // Use our size variable
            if (tree[i] == key) {
                return 1;   
            }
        }
        return 0;   
    }
};
