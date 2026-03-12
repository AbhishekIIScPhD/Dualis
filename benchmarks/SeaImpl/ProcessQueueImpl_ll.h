#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include "seahorn/seahorn.h"

#define MIN -129
#define MAX 128

class ProcessQueue {
private:
    struct Node {
        int time_to_wait;
        Node* next;
        
        Node(int t) : time_to_wait(t), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int current_size;

public:
    ProcessQueue() : head(nullptr), tail(nullptr), current_size(0) {}
    
    ProcessQueue(const std::vector<int>& elements) : head(nullptr), tail(nullptr), current_size(0) {
        for (int e : elements) {
            insert(e);
        }
    }

    // Destructor to free dynamically allocated memory
    ~ProcessQueue() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* next_node = curr->next;
            delete curr;
            curr = next_node;
        }
    }

    void insert(int nexttime) {
        Node* newNode = new Node(nexttime);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        current_size++;
    }

    int len() const {
        return current_size;
    }

    int minttw() const {
        if (head == nullptr) {
            return MAX;
        }
        
        int min_val = head->time_to_wait;
        Node* curr = head->next;
        
        while (curr != nullptr) {
            if (curr->time_to_wait < min_val) {
                min_val = curr->time_to_wait;
            }
            curr = curr->next;
        }
        return min_val;
    }

    void choosenext() {
        Node* curr = head;
        Node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->time_to_wait == 1) {
                Node* to_delete = curr;
                
                // Remove from head or middle/end
                if (prev == nullptr) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                
                // Update tail if we are deleting the last element
                if (curr == tail) {
                    tail = prev;
                }
                
                curr = curr->next;
                delete to_delete;
                current_size--;
            } else {
                curr->time_to_wait--;
                prev = curr;
                curr = curr->next;
            }
        }
    }

    bool isEmpty() const { 
        return head == nullptr; 
    }
    
    // Optional: Delete copy constructor and assignment operator 
    // to prevent shallow copy issues with raw pointers.
    ProcessQueue(const ProcessQueue&) = delete;
    ProcessQueue& operator=(const ProcessQueue&) = delete;
};
