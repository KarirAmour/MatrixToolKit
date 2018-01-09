#ifndef _VECTORINFO_H_
#define _VECTORINFO_H_

#include "info.h"
#include "vector.h"
#include <cstdlib>

#include <cstdlib>


template<typename T>
struct VectorInfo {

/*********** FLAGS *********/
	bool zero_flag;
	bool sum_flag;
	bool norm_flag;

	std::size_t zero_count;
	T vec_sum;
	T norm_sq;
/***************************/
    VectorInfo(bool zeroflg=false, bool sumflg=false, bool normflg=false, 
        	std::size_t zero=0, T sum=0.0, T norm=0.0);

	void countZeros(Vector<T> *vec);
	void calculateVecSum(Vector<T> *vec);
	void calculateSquaredNorm(Vector<T> *vec);

};

#endif
