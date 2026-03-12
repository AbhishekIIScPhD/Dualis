#include "../../Impl/ProcessQueueImpl.h"

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

    rc::check("testing abstract program", [&ceFile](ProcessQueue pq) {
        int nexttime = 1;
        int loop_count = *rc::gen::inRange(50, 100);

        for (int i = 0; i < loop_count; ++i) {
            bool nondet = *rc::gen::inRange(0, 2);

            if (nondet) {
                int len = pq.len();
                int min_ttw = pq.minttw();
                
                pq.insert(nexttime);
                
                int len1 = pq.len();
                int min_ttw1 = pq.minttw();

                bool expr_insert = (true);

                if (!expr_insert) {
                    ceFile << "(insert nexttime=" << nexttime 
                           << ", len=" << len
                           << ", len1=" << len1                           
                           << ", min_ttw=" << min_ttw 
                           << ", min_ttw1=" << min_ttw1 << ")\n";
                }
                RC_ASSERT(expr_insert);
                nexttime++;
            } else if (nexttime > 1) {
                int len = pq.len();
                int min_ttw = pq.minttw();
                
                pq.choosenext();
                
                int len1 = pq.len();
                int min_ttw1 = pq.minttw();

                bool expr_choosenext = (true);

                if (!expr_choosenext) {
                    ceFile << "(choosenext len=" << len
                           << ", len1=" << len1                      
                           << ", min_ttw=" << min_ttw
                           << ", min_ttw1=" << min_ttw1 << ")\n";
                }
                RC_ASSERT(expr_choosenext);
                nexttime--;
            }
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
