#ifndef _VECTORINFO_H_
#define _VECTORINFO_H_

#include "info.h"
#include <cstdlib>

class Vector;

struct VectorInfo {

/*********** FLAGS *********/
	bool zero_flag;
	bool sum_flag;
	bool norm_flag;

	std::size_t zero_count;
	TYPE vec_sum;
	TYPE norm_sq;
/***************************/
    VectorInfo(bool zeroflg=false, bool sumflg=false, bool normflg=false, 
        	std::size_t zero=0, TYPE sum=0.0, TYPE norm=0.0);

	void countZeros(Vector *vec);
	void calculateVecSum(Vector *vec);
	void calculateSquaredNorm(Vector *vec);

};

#endif
