#include "../../KleeImpl/BinaryTreeImpl.h"

int main(int argc, char* argv[])
{
  // prepareData(MIN, MAX);
  BinaryTree bt;
  init(bt);

  int min = bt.min();
  int isEmpty = bt.isEmpty();

  int v;
  klee_make_symbolic(&v, sizeof(v), "v");
  klee_assume(v < MAX);
  klee_assume(v > MIN);

  int ret1 = bt.search(v);
  bool expr = (false);
  if (!expr) {
    if (argc > 1) {
      const char *logfile = argv[1];
      FILE *f = fopen(logfile, "w");
      if (!f) {
        printf("Failed to open %s\n", logfile);
      } else {
        fprintf(f,
		"(search v=%d, min=%d, isEmpty=%d, ret1=%d)\n",
		klee_get_value_i32(v),
		klee_get_value_i32(min),
		klee_get_value_i32(isEmpty),
		klee_get_value_i32(ret1));
        fclose(f);
      }
    }
  }

  klee_assert(expr);
  return 0;
}
