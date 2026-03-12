#include "../../Impl/FlatHashSetImpl.h"

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

    rc::check("testing abstract program", [&ceFile](FlatHashSet fhs) {
        int N = *rc::gen::inRange(1, MAX);
        
        int i = 0;
        while (i < N) {
            int v = i;
            int len = fhs.len();
            bool containsv = fhs.contains(v);
            int min = fhs.minKey();
            int max = fhs.maxKey();

            fhs.insert(v);

            int len1 = fhs.len();
            bool containsv1 = fhs.contains(v);
            int min1 = fhs.minKey();
            int max1 = fhs.maxKey();

            bool expr_insert = (true);

            if (!expr_insert) {
                ceFile << "(insert v=" << v
		       << ", len=" << len
		       << ", containsv=" << containsv
                       << ", min=" << min
		       << ", max=" << max
		       << ", len1=" << len1 
                       << ", containsv1=" << containsv1
		       << ", min1=" << min1
		       << ", max1=" << max1
		       << ")\n";
            }
            RC_ASSERT(expr_insert);
            i++;
        }

        i = 0;
        while (i < N) {
            int v = i;
            int len = fhs.len();
            bool containsv = fhs.contains(v);
            int min = fhs.minKey();
            int max = fhs.maxKey();
            
            fhs.erase(v);

            int len1 = fhs.len();
            bool containsv1 = fhs.contains(v);
            int min1 = fhs.minKey();
            int max1 = fhs.maxKey();

            bool expr_erase = (true);

            if (!expr_erase) {
                ceFile << "(erase v=" << v
		       << ", len=" << len
		       << ", containsv=" << containsv
                       << ", min=" << min
		       << ", max=" << max
		       << ", len1=" << len1 
                       << ", containsv1=" << containsv1
		       << ", min1=" << min1
		       << ", max1=" << max1
		       << ")\n";
            }
            RC_ASSERT(expr_erase);
            i++;
        }

        int len = fhs.len();
        fhs.reserve(N);
        int len1 = fhs.len();
        
        bool expr_reserve = (true);

        if(!expr_reserve) {
            ceFile << "(reserve len=" << len
		   << ", N=" << N
		   << ", len1=" << len1
		   << ")\n";
        }
        RC_ASSERT(expr_reserve);

        i = 0;
        while (i < N) {
            int v = i + N;
            int len = fhs.len();
            bool containsv = fhs.contains(v);
            int min = fhs.minKey();
            int max = fhs.maxKey();

            fhs.insert1(v);

            int len1 = fhs.len();
            bool containsv1 = fhs.contains(v);
            int min1 = fhs.minKey();
            int max1 = fhs.maxKey();

            bool expr_insert1 = (true);

            if (!expr_insert1) {
                ceFile << "(insert1 v=" << v
		       << ", len=" << len
		       << ", containsv=" << containsv
                       << ", min=" << min
		       << ", max=" << max
		       << ", len1=" << len1 
                       << ", containsv1=" << containsv1
		       << ", min1=" << min1
		       << ", max1=" << max1
		       << ")\n";
            }
            RC_ASSERT(expr_insert1);
            i++;
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
