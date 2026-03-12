#include <algorithm> // For std::min
#include <vector>

#include "seahorn/seahorn.h"

using namespace std;

#define MIN 0
#define MAX 10

class TokenBucket {
private:
  int current_tokens_;

public:
  TokenBucket() = default;

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
