#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define MIN -10
#define MAX 11

class Stack {
private:
  std::vector<int> stack;

public:
  Stack() = default;

  // Use member initialization instead of pushing elements one by one
  Stack(const std::vector<int> elements) : stack(elements) {}

  // Rule of Five: Define copy constructor, copy assignment, move constructor, move assignment
  Stack(const Stack& other) = default;
  Stack& operator=(const Stack& other) = default;
  Stack(Stack&& other) = default;
  Stack& operator=(Stack&& other) = default;
  ~Stack() = default;
    
  void push(int key) {
    stack.push_back(key);
  }

  void pop() {
    if (!stack.empty()) {
      stack.pop_back();
    }
  }

  int len() const {
    return stack.size();
  }
};

constexpr uint8_t CMD_PUSH = 0x01;
constexpr uint8_t CMD_POP = 0x02;

constexpr int DATA_SIZE = 1000;

int data[DATA_SIZE];
int data_index = 0;

void prepareData(int min, int max) {
  srand(42); // Fixed seed for consistent fuzzing
    
  for (int i = 0; i < DATA_SIZE; i++) {
    data[i] = min + (rand() % (max - min + 1));
  }
  data_index = 0;
}

int getValue() {
  int value = data[data_index];
  data_index = (data_index + 1) % DATA_SIZE;
  return value;
}
