#include "../../KleeImpl/StackImpl.h"

int main(int argc, char *argv[])
{
  // prepareData(MIN, MAX);
  Stack st;
  init(st);

  int sl = st.len();
  // int v = getValue();
  st.pop();
  int sl1 = st.len();

  bool expr = (sl1 == sl + 1);

  if (!expr) {
    if (argc > 1) {
      const char *logfile = argv[1];
      FILE *f = fopen(logfile, "w");
      if (!f) {
        printf("Failed to open %s\n", logfile);
      } else {
        fprintf(f,
		"(po sl=%d, sl1=%d)\n",
		klee_get_value_i32(sl),
		klee_get_value_i32(sl1));
        fclose(f);
      }
    }
  }

  klee_assert(expr);
  return 0;
}
