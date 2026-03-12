#include "../../Impl/StackImpl.h"

#undef MIN
#undef MAX

#define MIN 100
#define MAX 1000

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Please provide a file path for logging." << std::endl;
    return 1;
  }
  std::string filePath = argv[1];
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile.is_open()) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  rc::check("testing abstract stack program", [&ceFile](Stack st) {

        
    int c = 0, d = 0;
    int N = *rc::gen::inRange(1, MAX);
        
    while (c < N) {
      int sl = st.len();
      st.push(N);
      int sl1 = st.len();
      bool expr_pu = (false);
      if (!expr_pu) {
	ceFile << "(pu N=" << N
	       << ", sl=" << sl
	       << ", sl1=" << sl1
	       << ")\n";
      }
      RC_ASSERT(expr_pu);
      c = c + 1;
    }

    while (st.len() != 0) {
      int sl = st.len();
      st.pop();
      int sl1 = st.len();
      // bool expr_pop = (((sl - sl1) <= 1) and ((sl - sl1) > 0));
      bool expr_po = (false);
      if (!expr_po) {
	ceFile << "(po sl=" << sl
	       << ", sl1=" << sl1
	       << ")\n";
      }
      RC_ASSERT(expr_po);
      d = d + 1;
    }
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
