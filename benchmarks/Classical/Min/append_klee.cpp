#include "../../KleeImpl/MinImpl.h"

int main(int argc, char* argv[])
{
  string filePath = argv[1];
  cout << "Final Path: " << filePath << endl;
  ofstream ceFile(filePath, ios::out);
  if (!ceFile) {
    cerr << "Error: Unable to open log file." << endl;
    return 1;
  }

  prepareData(MIN, MAX);
Min mn) {
    int lmin = mn.minElem();
    int v = *rc::gen::inRange(MIN, MAX);
    mn.append(v);
    int lmin1 = mn.minElem();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(append v = " << v
	       << ", lmin = " << lmin
	       << ", lmin1 = " << lmin1
	       << ")\n";
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
