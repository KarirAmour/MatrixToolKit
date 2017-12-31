#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "info.h"
#include <cstdlib>
#include <vector>

#define INIT_SIZE 8
#define CHECK_BOUNDS 1



class OutOfBoundsException {};

class Vector {
	
	std::size_t vec_size;
	std::size_t capacity;

	/******* FLAGS ******/
	bool flags_valid;
	bool is_zero;
	bool is_basis;
	bool sum_flag;
	bool norm_sq_flag;

	/******* INFO *******/

	TYPE *vec_data;


	TYPE vec_sum;
	TYPE norm_sq;
	std::size_t zero_count;

	/***** METHODS ******/

	void allocateData(std::size_t amount);
	void resize(std::size_t amount);

	void updateFlags();
	void calculateVecSum();
	void calculateSquaredNorm();
	void calculateIsBasis();
	void calculateIsZero();

	// Updates flags based on element TO BE added
	// If added element first, forced to check ENTIRE vector
	void elementUpdateFlags(std::size_t index, TYPE ele);

	// get&set elements without needing to update flags at each access.
	TYPE &operator[] (std::size_t index);


public:

	Vector(std::size_t length);
	Vector(std::size_t length, TYPE init);
	Vector(const Vector &vec);
	Vector(std::vector<TYPE> &vec);
	~Vector();

	bool operator==(const Vector &rhs) const;
	bool operator!=(const Vector &rhs) const;

	TYPE get(std::size_t index) const;
	TYPE set(std::size_t index, TYPE value);


	TYPE Sum();
	TYPE squaredNorm();

	void append(TYPE ele);

	bool isBasisVector();
	bool isZeroVector();

	void print() const;

	std::size_t getCapacity() const { return this->capacity; }
	std::size_t size() const { return this->vec_size; }

	
};


// struct VectorInfo {
// /*********** FLAGS *********/
// 	bool flagsValid;
// 	bool isZero;
// 	bool isBasis;

// 	bool vecSumUpdated;
// 	bool normSqUpdated;
// 	TYPE vecSum;
// 	TYPE normSq;
// /***************************/
// 	VectorInfo() : 
// 	flagsValid{false}, isZero{false}, isBasis{false}, 
// 	vecSumUpdated{false}, normSqUpdated{false} {}

// 	void calculateVecSum();
// 	void calculateSquaredNorm();
// 	void calculateIsBasis();
// 	void calculateIsZero();
// };


#endif

