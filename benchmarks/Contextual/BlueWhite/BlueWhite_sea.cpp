#include "../../SeaImpl/BlueWhiteImpl.h"

extern int nd();

int main(int argc, char* argv[]) {
  BWList bwl;
  int N = nd(), bcount = 0, inserted_blue = 0;
  
  __VERIFIER_assume(N >= 0);

  for (int i = 0; i < N; ++i) {
    int color = nd() % 2; // 0 or 1
    if (color == 0 && !inserted_blue) {
      bwl.push(color);
      bcount = bwl.blue_count();
      inserted_blue = bwl.blue_present();
    } else if (color == 1) {
      bwl.push1(color);
      bcount = bwl.blue_count();
    }
  }

  sassert(inserted_blue != 1 or bcount == 1);
  return 0;
}
