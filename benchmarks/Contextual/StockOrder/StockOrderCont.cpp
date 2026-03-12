#include "../../Impl/StockOrderImpl.h"

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

    rc::check("testing abstract program", [&ceFile](StockOrder so) {
        int loop_count = *rc::gen::inRange(0, 20);

        for (int i = 0; i < loop_count; ++i) {
            int stock = *rc::gen::inRange(-10, 11);
            int order = *rc::gen::inRange(-10, 11);

            if (stock >= 0 && order >= 0 && order <= stock) {
                int len = so.len();
                int minDiff = so.getMinDiff();

                so.addStockOrder(std::make_pair(stock, order));

                int len1 = so.len();
                int minDiff1 = so.getMinDiff();

                bool expr_addStockOrder = (true);

                if (!expr_addStockOrder) {
                    ceFile << "(addStockOrder stock=" << stock
			   << ", order=" << order
                           << ", len=" << len
                           << ", len1=" << len1
			   << ", minDiff=" << minDiff
			   << ", minDiff1=" << minDiff1
			   << ")\n";
                }
                RC_ASSERT(expr_addStockOrder);
            }
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
