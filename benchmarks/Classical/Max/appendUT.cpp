#include "../../Impl/MaxImpl.h"


int main(int argc, char* argv[])
{
  string filePath = argv[1];
  cout << "Final Path: " << filePath << endl;
  ofstream ceFile(filePath, ios::out);
  if (!ceFile) {
    cerr << "Error: Unable to open log file." << endl;
    return 1;
  }

  rc::check("testing Max",[&ceFile](Max mx) {
    int lmax = mx.maxElem();
    int v = *rc::gen::inRange(MIN, MAX);
    mx.append(v);
    int lmax1 = mx.maxElem();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(append v = " << v
	       << ", lmax = " << lmax
	       << ", lmax1 = " << lmax1
	       << ")\n";
      }
    }
    RC_ASSERT(expr);
  });
  ceFile.close();
  return 0;
}
