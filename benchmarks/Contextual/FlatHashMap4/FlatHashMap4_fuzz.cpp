#include "../../FuzzImpl/FlatHashMapImpl.h"

int main(int argc, char *argv[]) {
  bool fuzzer_mode = getenv("FUZZING") != nullptr;
  std::string filePath = argv[1];
    
  std::ofstream ceFile(filePath, std::ios::app);

  while (__AFL_LOOP(10000)) {
    std::vector<uint8_t> fuzzBuf(4096);
    ssize_t fuzzLen = read(0, fuzzBuf.data(), fuzzBuf.size());

    if (fuzzLen <= 10) {
      continue;
    }

    FlatHashMap fhm;

    int8_t N;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -1, N);
    if (N <= 0){
      continue;
    }

    int i;
    READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -2, i);
    if (i != 0){
      continue;
    }
    while (i < N) {
      DECLARE_FHM_INSERT_STATE_VARS();
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 3, k);
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 4, v);
      if (k == i && v == i){

	FHM_INSERT_WITH_STATE(fhm, k, v);

	bool expr_insert = (true);

	if (!expr_insert) {
	  LOG_FHM_INSERT_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_insert);
      }
      i++;
    }

    READ_INT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -5, i);
    if (i != 0){
      continue;
    }
       
    int gflag;
    READ_INT8_FROM_FUZZBUF(fuzzBuf, fuzzLen -6, gflag);
    if (gflag != 0){
      continue;
    }
    while(i < N) {
      DECLARE_FHM_ERASE_STATE_VARS();
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 7 , flag);
      READ_UINT8_FROM_FUZZBUF(fuzzBuf, fuzzLen - 8 , k);
      if (gflag == flag && k == i){
	FHM_ERASE_WITH_STATE(fhm, k, flag);
	    
	bool expr_erase = (true);
            
	if (!expr_erase) {
	  LOG_FHM_ERASE_STATE(ceFile, fuzzer_mode);
	}
	assert(expr_erase);
      }

      i++;
      gflag = 1 - gflag; 
    }
        
    fuzzBuf.clear();
  }

  ceFile.close();
  return 0;
}
