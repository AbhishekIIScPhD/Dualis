#include <set>       // The header for std::multiset
#include <utility>   // For std::pair

extern int nd();

int main(int argc, char* argv[])
{
  std::multiset<int> set;
  int N;

  __VERIFIER_assume(N >= 0);

  // Initialize the multiset with N non-deterministic values
  for (int i = 0; i < N; i++) {
    int v = nd();
    set.emplace(v);
  }

  // First operation sequence: emplace multiple values
  int N2;
  __VERIFIER_assume(N2 >= 0);
  
  for (int i = 0; i < N2; i++) {
    int v = 1;
    
    int countvo = set.count(1);
    int countvt = set.count(2);
    int len = set.size();

    set.emplace(v);

    int countvo1 = set.count(1);
    int countvt1 = set.count(2);
    int len1 = set.size();

    bool expr_emplace = (true);
    sassert(expr_emplace);
  }

  // Second operation: emplace value 2
  int v = 2;
  int countvo = set.count(1);
  int countvt = set.count(v);
  int len = set.size();

  set.emplace(v);

  int countvo1 = set.count(1);
  int countvt1 = set.count(v);
  int len1 = set.size();
  
  bool expr_emplace1 = (true);
  sassert(expr_emplace1);

  return 0;
}
