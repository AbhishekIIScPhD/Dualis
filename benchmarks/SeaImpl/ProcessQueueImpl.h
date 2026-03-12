#include "seahorn/seahorn.h"
#include <iostream>
#include <vector>

#define MIN -10
#define MAX 10
#define MAX_PROC 1000
#define NULL_IDX -1

class ProcessQueue {
private:
    struct Node {
        int time_to_wait;
        int next_idx;
    };

    // Statically allocated memory pool 
    Node memory_pool[MAX_PROC];

    int head;
    int tail;
    int free_head; // Points to the next available slot in the array
    int current_size;

public:
    ProcessQueue() {
        head = NULL_IDX;
        tail = NULL_IDX;
        current_size = 0;

        // Initialize the free list to link all available slots sequentially
        for (int i = 0; i < MAX_PROC - 1; ++i) {
            memory_pool[i].next_idx = i + 1;
        }
        memory_pool[MAX_PROC - 1].next_idx = NULL_IDX;
        free_head = 0;
    }

    // Optional: Constructor for initialization from vector
    ProcessQueue(const std::vector<int>& elements) : ProcessQueue() {
        for (int e : elements) {
            insert(e);
        }
    }

    void insert(int nexttime) {
        // Prevent out-of-bounds array access if capacity is reached
        if (free_head == NULL_IDX) {
            return;
        }

        // Pop a free node from the free list
        int new_idx = free_head;
        free_head = memory_pool[free_head].next_idx;

        // Initialize the new node
        memory_pool[new_idx].time_to_wait = nexttime;
        memory_pool[new_idx].next_idx = NULL_IDX;

        // Link it to the end of the queue
        if (head == NULL_IDX) {
            head = tail = new_idx;
        } else {
            memory_pool[tail].next_idx = new_idx;
            tail = new_idx;
        }
        current_size++;
    }

    int len() const {
        return current_size;
    }

    int minttw() const {
        if (head == NULL_IDX) {
            return MAX;
        }

        int min_val = memory_pool[head].time_to_wait;
        int curr = memory_pool[head].next_idx;

        while (curr != NULL_IDX) {
            if (memory_pool[curr].time_to_wait < min_val) {
                min_val = memory_pool[curr].time_to_wait;
            }
            curr = memory_pool[curr].next_idx;
        }
        return min_val;
    }

    void choosenext() {
        int curr = head;
        int prev = NULL_IDX;

        while (curr != NULL_IDX) {
            if (memory_pool[curr].time_to_wait == 1) {
                int to_delete = curr;

                // Unlink from the queue
                if (prev == NULL_IDX) {
                    head = memory_pool[curr].next_idx;
                } else {
                    memory_pool[prev].next_idx = memory_pool[curr].next_idx;
                }

                // Update tail if necessary
                if (curr == tail) {
                    tail = prev;
                }

                curr = memory_pool[curr].next_idx;

                // Push the deleted node back onto the free list
                memory_pool[to_delete].next_idx = free_head;
                free_head = to_delete;
                current_size--;
            } else {
                memory_pool[curr].time_to_wait--;
                prev = curr;
                curr = memory_pool[curr].next_idx;
            }
        }
    }

    bool isEmpty() const {
        return head == NULL_IDX;
    }
};
