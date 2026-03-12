#include "../../Impl/TokenBucketImpl.h"

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

    rc::check("testing abstract program", [&ceFile](TokenBucket tb) {

        
        int b_size = *rc::gen::inRange(1, MAX);
        int c_rate = *rc::gen::inRange(1, MAX);

        RC_PRE(b_size > 0);
        RC_PRE(c_rate > 0);
        RC_PRE(b_size >= c_rate);

        int avai_tokens = tb.getAvailableTokens();
        tb.generateTokens(b_size);
        int avai_tokens1 = tb.getAvailableTokens();

        bool expr_generateTokens = (true);
        if (!expr_generateTokens) {
            ceFile << "(generateTokens b_size=" << b_size 
                   << ", avai_tokens1=" << avai_tokens1
		   << ")\n";
        }
        RC_ASSERT(expr_generateTokens);

        int consumed_tokens = 0;
        avai_tokens = tb.getAvailableTokens();

        while (avai_tokens >= c_rate) {
            tb.consume(c_rate);
            int avai_tokens1 = tb.getAvailableTokens();
            
            bool expr_consume = (true);
            if (!expr_consume) {
                ceFile << "(consume c_rate=" << c_rate
		       << ", avai_tokens=" << avai_tokens
                       << ", avai_tokens1=" << avai_tokens1
		       << ")\n";
            }
            RC_ASSERT(expr_consume);
            
            consumed_tokens += c_rate;
            avai_tokens = avai_tokens1;
        }
    });

    ceFile.close();
    std::cout << "Tests completed. Log written to " << filePath << std::endl;
    return 0;
}
