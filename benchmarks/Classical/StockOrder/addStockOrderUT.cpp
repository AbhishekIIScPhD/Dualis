#include "../../Impl/StockOrderImpl.h"


int main(int argc, char* argv[])
{
  string filePath = argv[1];
  cout << "Final Path: " << filePath << endl;
  ofstream ceFile(filePath, ios::out);
  if (!ceFile) {
    cerr << "Error: Unable to open log file." << endl;
    return 1;
  }

  bool success = rc::check("testing StockOrder", [&ceFile](StockOrder obj) {
    int len = obj.len();
    int stock = *rc::gen::inRange(MIN, MAX);
    int order = *rc::gen::inRange(MIN, MAX);
    int minDiff = obj.getMinDiff();
    auto p =  make_pair(stock, order);
    obj.addStockOrder(p);
    int len1 = obj.len();
    int minDiff1 = obj.getMinDiff();
    bool expr = ((stock - minDiff) > (- 100));
    if(!expr){
      if(ceFile.is_open()){
	ceFile	<< "(addStockOrder stock = " << stock
		<< ", order = " << order
		<< ", len = " << len
		<< ", len1 = " << len1
		<< ", minDiff = " << minDiff
		<< ", minDiff1 = " << minDiff1
		<< ")\n";
      }
      ceFile.close();
    }
    RC_ASSERT(expr);
  });
  return 0;
}
