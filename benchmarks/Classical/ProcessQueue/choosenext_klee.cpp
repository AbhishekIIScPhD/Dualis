#include "../../KleeImpl/ProcessQueueImpl.h"

int main(int argc, char* argv[])
{
  std::string filePath = argv[1];
  std::cout << "Final Path: " << filePath << std::endl;
  std::ofstream ceFile(filePath, std::ios::out);
  if (!ceFile) {
    std::cerr << "Error: Unable to open log file." << std::endl;
    return 1;
  }

  prepareData(MIN, MAX);
ProcessQueue pq) {
    int min_ttw = pq.minttw();
    int len = pq.len();
    pq.choosenext();
    int min_ttw1 = pq.minttw();
    int len1 = pq.len();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(choosenext len = " << len
	       << ", len1 = " << len1
	       << ", min_ttw = " << min_ttw
	       << ", min_ttw1 = " << min_ttw1
	       << ")\n";
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
