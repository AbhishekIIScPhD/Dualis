#include "../../Impl/NormalFilterQueueImpl.h"

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

  rc::check("testing abstract program", [&ceFile](NormalFilterQueue nfq) {
    int loop_count = *rc::gen::inRange(10, 50);

    for (int i = 0; i < loop_count; ++i) {
      int packetSize = *rc::gen::inRange(MIN, MAX);
      int prio = *rc::gen::inRange(0, 2);

      if (packetSize > 0) {
	if (prio == 0 && packetSize >= 500) {
	  int len = nfq.len();
	  int maxPrio = nfq.maxPrio();
	  int minPacketSize = nfq.minPacketSize();

	  nfq.append(std::make_pair(prio, packetSize));

	  int len1 = nfq.len();
	  int maxPrio1 = nfq.maxPrio();
	  int minPacketSize1 = nfq.minPacketSize();

	  bool expr_append = (true);

	  if (!expr_append) {
	    ceFile << "(append prio=" << prio
		   << ", packetSize=" << packetSize
		   << ", len=" << len
		   << ", maxPrio=" << maxPrio
		   << ", minPacketSize=" << minPacketSize
		   << ", len1=" << len1
		   << ", maxPrio1=" << maxPrio1
		   << ", minPacketSize1=" << minPacketSize1 << ")\n";
	  }
	  RC_ASSERT(expr_append);
	}
      }

      int len = nfq.len();
      int maxPrio = nfq.maxPrio();
      int minPacketSize = nfq.minPacketSize();
	    
      nfq.processQueue();

      int len1 = nfq.len();
      int maxPrio1 = nfq.maxPrio();
      int minPacketSize1 = nfq.minPacketSize();

      bool expr_processQueue = (true);

      if (!expr_processQueue) {
	ceFile << "(processQueue len=" << len
	       << ", maxPrio=" << maxPrio
	       << ", minPacketSize=" << minPacketSize
	       << ", len1=" << len1
	       << ", maxPrio1=" << maxPrio1
	       << ", minPacketSize1=" << minPacketSize1 << ")\n";
      }
      RC_ASSERT(expr_processQueue);
        }
  });

  ceFile.close();
  std::cout << "Tests completed. Log written to " << filePath << std::endl;
  return 0;
}
