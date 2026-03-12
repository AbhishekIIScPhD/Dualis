#include "../../Impl/StackImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << "\n";
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << "\n";
    return 1;
  }

  bool success = rc::check("testing Stack", [&ceFile](Stack obj) {
    int sl = obj.len();
    short N = *rc::gen::inRange(MIN, MAX);
    obj.push(N);
    int sl1 = obj.len();
    bool expr = (((sl1 - sl) > 0) and ((sl1 - sl) <= 1));
    if(!expr){
      if(ceFile.is_open()){
	ceFile << "(pu N = " << N
	       << ", sl = " << sl
	       << ", sl1 = " << sl1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
