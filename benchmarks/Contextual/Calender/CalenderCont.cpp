#include "../../Impl/CalenderImpl.h"

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

    rc::check("testing abstract program", [&ceFile](Cal cal) {
        int N = *rc::gen::inRange(5, 100);

        for (int i = 0; i < N; ++i) {
            int ev1 = *rc::gen::inRange(MIN, MAX);
            int ev2 = *rc::gen::inRange(MIN, MAX);

            if (((ev1 >= 0 && ev1 <= 3) && (ev2 >= 0 && ev2 <= 3)) &&
                ((ev1 - ev2) < 2)) {
                
                int len = cal.len();
                int maxDiff = cal.maxDiff();
                cal.insert(std::make_pair(ev1, ev2));
                
                int len1 = cal.len();
                int maxDiff1 = cal.maxDiff();
                
                bool expr_insert = ((maxDiff <= 1) and ((maxDiff1 - maxDiff) <= (- 1)) and (len <= 0));

                if (!expr_insert) {
                    ceFile << "(insert len=" << len
			   << ", len1=" << len1 
			   << ", ev1=" << ev1
			   << ", ev2=" << ev2
                           << ", maxDiff=" << maxDiff
                           << ", maxDiff1=" << maxDiff1 << ")\n";
                }
                RC_ASSERT(expr_insert);
            }
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
