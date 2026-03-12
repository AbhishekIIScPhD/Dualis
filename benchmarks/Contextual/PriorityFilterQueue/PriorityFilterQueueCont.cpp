#include "../../Impl/PriorityFilterQueueImpl.h"

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

    rc::check("testing abstract program", [&ceFile](PriorityFilterQueue pfq) {
        int loop_count = *rc::gen::inRange(0, 20);

        for (int i = 0; i < loop_count; ++i) {
            int packetSize = *rc::gen::inRange(0, 511);
            int prio = *rc::gen::inRange(0, 2);

            if (packetSize > 0) {
                if (prio == 1 && packetSize < 500) {
                    int len = pfq.len();
                    int minPrio = pfq.minPrio();
                    int maxPacketSize = pfq.maxPacketSize();

                    pfq.append(std::make_pair(prio, packetSize));

                    int len1 = pfq.len();
                    int minPrio1 = pfq.minPrio();
                    int maxPacketSize1 = pfq.maxPacketSize();

                    bool expr_append = (true);

                    if (!expr_append) {
                        ceFile << "(append prio=" << prio
			       << ", packetSize=" << packetSize
                               << ", len=" << len
			       << ", minPrio=" << minPrio
                               << ", maxPacketSize=" << maxPacketSize
                               << ", len1=" << len1
			       << ", minPrio1=" << minPrio1
                               << ", maxPacketSize1=" << maxPacketSize1
			       << ")\n";
                    }
                    RC_ASSERT(expr_append);
                }
            }
        }

        int len = pfq.len();
        int minPrio = pfq.minPrio();
        int maxPacketSize = pfq.maxPacketSize();

        pfq.processQueue();

        int len1 = pfq.len();
        int minPrio1 = pfq.minPrio();
        int maxPacketSize1 = pfq.maxPacketSize();

        bool expr_processQueue = (true);

        if (!expr_processQueue) {
            ceFile << "(processQueue len=" << len
		   << ", minPrio=" << minPrio
                   << ", maxPacketSize=" << maxPacketSize
                   << ", len1=" << len1
		   << ", minPrio1=" << minPrio1
                   << ", maxPacketSize1=" << maxPacketSize1
		   << ")\n";
        }
        RC_ASSERT(expr_processQueue);

    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
