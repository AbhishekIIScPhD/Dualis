#include "../../KleeImpl/PriorityFilterQueueImpl.h"

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
PriorityFilterQueue obj) {
    int maxPacketSize = obj.maxPacketSize();
    int minPrio = obj.minPrio();
    int len = obj.len();
    int prio = 1;
    int packetSize = *rc::gen::inRange(MINPACK, MAXPACK);
    obj.append(make_pair(prio, packetSize));
    int maxPacketSize1 = obj.maxPacketSize();
    int minPrio1 = obj.minPrio();
    int len1 = obj.len();
    bool expr = (false);
    if(!expr){
      if (ceFile.is_open()){
	ceFile << "(append prio = " << prio
	       << ", packetSize = " << packetSize
	       << ", len = " << len
	       << ", minPrio = " << minPrio
	       << ", maxPacketSize = " << maxPacketSize
	       << ", len1 = " << len1
	       << ", minPrio1 = " << minPrio1
	       << ", maxPacketSize1 = "<< maxPacketSize1
	       << ")\n";
      }
    }
    klee_assert(expr);
  
  ceFile.close();
  return 0;
}
