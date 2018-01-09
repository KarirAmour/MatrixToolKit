#include "vectorinfo.h"
#include "vector.h"
#include <iostream>

VectorInfo::VectorInfo(bool zeroflg, bool sumflg, bool normflg, 
		std::size_t zero, TYPE sum, TYPE norm) :
    zero_flag{zeroflg}, sum_flag{sumflg}, norm_flag{normflg}, 
    zero_count{zero}, vec_sum{sum}, norm_sq{norm} {}

// All three methods use loop unrolling
//  Though may be more efficient to create one method called updateAllInfo(),
//   and loop through vector all in one   
void VectorInfo::countZeros(Vector *vec) {

	// By subtracting the number of zeros, we save on
	// this->data[i] != 0 checks (not sure if actually true)
	std::size_t i = 0;
	this->zero_count = vec->vec_size;
	for (; (i - 4) < vec->vec_size; ++i) {
		if (vec->vec_data[i + 0]) --this->zero_count;
		if (vec->vec_data[i + 1]) --this->zero_count;
		if (vec->vec_data[i + 2]) --this->zero_count;
		if (vec->vec_data[i + 3]) --this->zero_count;
	}
	for (; i < vec->vec_size; ++i) if (vec->vec_data[i]) --this->zero_count;
    this->zero_flag = true;
}

// Sums the elements in vector
void VectorInfo::calculateVecSum(Vector *vec) {

	this->vec_sum = 0;
	std::size_t i = 0;    
	for (; (i - 4) < vec->vec_size; ++i) {
		this->vec_sum += vec->vec_data[i + 0];
		this->vec_sum += vec->vec_data[i + 1];
		this->vec_sum += vec->vec_data[i + 2];
		this->vec_sum += vec->vec_data[i + 3];
	}
	for (; i < vec->vec_size; ++i) this->vec_sum += vec->vec_data[i];
    this->sum_flag = true;
    
}

// Can optimize by combining this with Sum/Zero/Basis.
// Will cross that bridge when in optimize phase.
void VectorInfo::calculateSquaredNorm(Vector *vec) {
	std::cout << "calcNS" << std::endl;
	this->norm_sq = 0;
	std::size_t i = 0;    
	for (; (i - 4) < vec->vec_size; ++i) {
		this->norm_sq += vec->vec_data[i + 0] * vec->vec_data[i + 0];
		this->norm_sq += vec->vec_data[i + 1] * vec->vec_data[i + 1];
		this->norm_sq += vec->vec_data[i + 2] * vec->vec_data[i + 2];
		this->norm_sq += vec->vec_data[i + 3] * vec->vec_data[i + 3];
	}
	for (; i < vec->vec_size; ++i) {
		this->norm_sq += vec->vec_data[i] * vec->vec_data[i];
	}
    this->norm_flag = true;
}
