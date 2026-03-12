#include "../../KleeImpl/CalenderImpl.h"


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
Cal cal) {
    int len = cal.len();
    int ev1 = *rc::gen::inRange(MIN, MAX);
    int ev2 = *rc::gen::inRange(MIN, MAX);
    int maxDiff = cal.maxDiff();
    auto p =  make_pair(ev1, ev2);
    cal.insert(p);
    int len1 = cal.len();
    int maxDiff1 = cal.maxDiff();
    bool expr = (false);
    if(!expr){
      if(ceFile.is_open()){
	ceFile << "(insert len = " << len
	       << ", len1 = " << len1
	       << ", ev1 = " << ev1
	       << ", ev2 = " << ev2
	       << ", maxDiff = " << maxDiff
	       << ", maxDiff1 = " << maxDiff1
	       << ")\n" << endl;
      }
      ceFile.close();
    }
    klee_assert(expr);
  
  return 0;
}
