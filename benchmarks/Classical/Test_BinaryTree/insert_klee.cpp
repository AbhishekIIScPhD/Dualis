#include "../../KleeImpl/BinaryTreeImpl.h"

int main(int argc, char* argv[])
{
  BinaryTree bt;
  init(bt);

  int min = bt.min();
  int isEmpty = bt.isEmpty();

  int n;
  klee_make_symbolic(&n, sizeof(n), "n");
  klee_assume(n < MAX);
  klee_assume(n > MIN);

  bt.insert(n);

  int min1 = bt.min();
  int isEmpty1 = bt.isEmpty();
  bool expr = (false);

  if (!expr) {
    if (argc > 1) {
      const char *logfile = argv[1];
      FILE *f = fopen(logfile, "w");
      if (!f) {
        printf("Failed to open %s\n", logfile);
      } else {
        fprintf(f,
		"(insert n=%d, min=%d, min1=%d, isEmpty=%d, isEmpty1=%d)\n",
		klee_get_value_i32(n),
		klee_get_value_i32(min),
		klee_get_value_i32(min1),		
		klee_get_value_i32(isEmpty),
		klee_get_value_i32(isEmpty1));
        fclose(f);
      }
    }
  }

  klee_assert(expr);
  return 0;
}
