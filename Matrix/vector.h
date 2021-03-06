#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "info.h"
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define INIT_SIZE 32
#define CHECK_BOUNDS 1

std::size_t nextPowerOfTwo(std::size_t value);
class CalculationError;
class OutOfBoundsException;
class DivisionByZeroError;

class Vector {
	// Need to add a flag/value for the index for the first nonzero element.	
	struct VectorInfo {

		/***** FLAGS *****/

		bool zero_flag;
		bool nonzero_flag;
		bool sum_flag;
		bool norm_flag;

		int first_nonzero; // First non zero index
		std::size_t zero_count;
		double vec_sum;
		double norm_sq;
		/*****************/
		
		VectorInfo(bool zeroflg=false, bool sumflg=false, bool normflg=false, 
       	std::size_t zero=0, double sum=0.0, double norm=0.0) :
			zero_flag{zeroflg}, sum_flag{sumflg}, norm_flag{normflg}, 
			zero_count{zero}, vec_sum{sum}, norm_sq{norm} {}

		//  It may be more efficient to create one method called updateAllInfo(),
		//  and loop through vector all in one   
		void countZeros(Vector *vec) {

			std::size_t i = 0;
			this->zero_count = 0;
			// Find first non zero index
			while (i < vec->size() and vec->vec_data[i] == 0) ++i;

			if (i == vec->size()) {
				this->first_nonzero = -1;
			} else {
				this->first_nonzero = i;
			}

			for (; i < vec->size(); ++i) {
				if (vec->vec_data[i]) --this->zero_count;
			}
			this->zero_flag = true;
		}

		// Sums the elements in vector
		void calculateVecSum(Vector *vec) {

			this->vec_sum = 0;
			for (std::size_t i = 0; i  < vec->size(); ++i) {
				this->vec_sum += vec->vec_data[i];
			}
			this->sum_flag = true;
			
		}

		// Can optimize by combining this with Sum/Zero/Basis.
		// Will cross that bridge when in optimize phase.
		void calculateSquaredNorm(Vector *vec) {
			this->norm_sq = 0;
			for (std::size_t i = 0; i < vec->size(); ++i) {
				this->norm_sq += vec->vec_data[i] * vec->vec_data[i];
			}
			this->norm_flag = true;
		}
		
	};


	/******* INFO *******/
	std::size_t vec_size;
	std::size_t capacity;

	double *vec_data;
	VectorInfo *info;

	/***** METHODS ******/

	void allocateData(std::size_t amount);
	void resize(std::size_t amount);

	// Updates flags based on element TO BE added
	// If added element first, forced to check ENTIRE vector
	void elementUpdateFlags(std::size_t index, double ele);

	// get&set elements without needing to update flags at each access.
	void swap(Vector &first, Vector &second) noexcept;

public:

	Vector(std::size_t length = 0);
	Vector(std::size_t length, double init);
	Vector(const Vector &vec);
	Vector(std::vector<double> &vec);

	~Vector();

	Vector &operator=(Vector other);
	bool operator==(const Vector &rhs) const;
	bool operator!=(const Vector &rhs) const;

	double operator*(const Vector &rhs) const;

	Vector &operator+=(const Vector &rhs);
	Vector &operator-=(const Vector &rhs);
	Vector operator+(Vector rhs);
	Vector operator-(Vector rhs);
	
	Vector &operator+=(const double rhs);
	Vector &operator-=(const double rhs);
	Vector &operator*=(const double rhs);
	Vector &operator/=(const double rhs);


	double &operator[] (std::size_t index);
	double get(std::size_t index) const;
	double set(std::size_t index, double value);


	double sum();
	double squaredNorm();

	void append(double ele);

	bool isBasisVector();
	bool isZeroVector();

	void print() const;

	std::size_t getCapacity() const { return this->capacity; }
	std::size_t size() const { return this->vec_size; }
	
};




#endif

