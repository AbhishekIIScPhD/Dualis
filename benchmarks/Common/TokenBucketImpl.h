#include <iostream>
#include <fstream>
#include <algorithm> // For std::min
#include <vector>
#include <filesystem>

using namespace std;

#define MIN 0
#define MAX 10

class TokenBucket {
private:
  int current_tokens_;

public:
  TokenBucket(int initial_tokens) : current_tokens_(initial_tokens) {}

  void generateTokens( int burst_size) {
    int new_tokens = burst_size;
    int current_tokens = current_tokens_;
    int total_tokens = current_tokens + new_tokens;
        
    current_tokens_ = std::min(burst_size, total_tokens);
  }

  bool consume(int consume_rate) {
    if (current_tokens_ >= consume_rate) {
      current_tokens_ -= consume_rate;
      return true;
    }
    return false;
  }

  int getAvailableTokens() const {
    return current_tokens_;
  }
};

constexpr uint8_t CMD_GENERATE = 0x01;
constexpr uint8_t CMD_CONSUME = 0x02;

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

void init (AlternatingList &al, unsigned char *buf, size_t buf_size){
  data_index = 0;
  ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
  if (len <= 0) {
    continue;
  }

  for (ssize_t i = 0; i < len; ++i) {
    uint8_t command = buf[i];

    switch (command) {
    case CMD_GENERATE: {
      int v = getValue();
      al.generateTokens(v);
      break;
    }
    case CMD_CONSUME: {
      int v = getValue();
      al.consume(v);
      break;
    }
    default:
      break;
    }
  }
}
