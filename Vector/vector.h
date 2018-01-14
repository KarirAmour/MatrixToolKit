#ifndef _VECdoubleOR_H_
#define _VECdoubleOR_H_

#include "info.h"
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define INIdouble_SIZE 32
#define CHECK_BOUNDS 1

std::size_t nextPowerOfdoublewo(std::size_t value);
class CalculationError;
class OutOfBoundsException;
class DivisionByZeroError;

class Vector {
		
	struct VectorInfo {

	/*********** FLAGS *********/
		bool zero_flag;
		bool sum_flag;
		bool norm_flag;

		std::size_t zero_count;
		double vec_sum;
		double norm_sq;
	/***************************/
		
		VectorInfo(bool zeroflg=false, bool sumflg=false, bool normflg=false, 
       	std::size_t zero=0, double sum=0.0, double norm=0.0) :
			zero_flag{zeroflg}, sum_flag{sumflg}, norm_flag{normflg}, 
			zero_count{zero}, vec_sum{sum}, norm_sq{norm} {}

		// All three methods use loop unrolling
		//  Lhough may be more efficient to create one method called updateAllInfo(),
		//   and loop through vector all in one   
		void countZeros(Vector *vec) {

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
		void calculateVecSum(Vector *vec) {

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
		void calculateSquaredNorm(Vector *vec) {
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
		
	};


	/******* INFO *******/

	std::size_t vec_size;
	std::size_t capacity;

	double *vec_data;
	VectorInfo *info;

	/***** MEdoubleHODS ******/

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




/******************************* CONSdoubleRUCdoubleORS ********************************/

Vector::Vector(std::size_t length) : 
		vec_size{length}, capacity{0}, info{new VectorInfo} {
	this->allocateData(length);
}


Vector::Vector(std::size_t length, double init) : vec_size{length}, capacity{0}, 
		info(new VectorInfo(true, true, true, init == 0.0f ? length : 0.0f, 
							init*length, init*init*length)) {

	this->allocateData(length);
	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = init;
	}
}


Vector::Vector(std::vector<double> &vec) {

	this->info = new VectorInfo();
	std::size_t length = vec.size();
	this->allocateData(length);

	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = vec[i];
		this->info->vec_sum += this->vec_data[i];
		this->info->norm_sq += this->vec_data[i] * this->vec_data[i];
		if (this->vec_data[i] == 0) {
			++this->info->zero_count;
		}
	}
	this->info->zero_flag = true;
	this->info->sum_flag = true;
	this->info->norm_flag = true;

}

// Copy Constructor
Vector::Vector(const Vector &other) : 
	vec_size{other.vec_size}, capacity{other.capacity}, 
	info{new VectorInfo(other.info)} {

	this->allocateData(this->vec_size);

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->vec_data[i] = other.vec_data[i];
	}
}

// Overloading swap to optimize copy and move assignment
void Vector::swap(Vector &first, Vector &second) noexcept {
	using std::swap;

	swap(first.vec_size, second.vec_size);
	swap(first.capacity, second.capacity);
	swap(first.vec_data, second.vec_data);
	swap(first.info, second.info);

}


// Copy Assigment
Vector &Vector::operator=(Vector other) {

	swap(*this, other);
	return *this;
}

// Destructor
Vector::~Vector() {
	delete this->info;
	delete[] this->vec_data;
}


/****************************** DAdoubleA ALLOCAdoubleION *****************************/

// Should only be called in constructor, else resize.
void Vector::allocateData(std::size_t amount) {
	this->vec_size = amount;
	this->capacity = amount ? nextPowerOfdoublewo(amount) : INIdouble_SIZE;
	this->vec_data = new double[this->capacity];

}

// Can only increase size
// Should enable ability to decrease size?
void Vector::resize(std::size_t amount) {

	std::size_t new_capacity = nextPowerOfdoublewo(this->vec_size + amount);
	if (this->capacity >= new_capacity) { 
		return;
	}

	this->capacity = new_capacity;
	double *temp = new double[this->capacity];
	std::memcpy(temp, this->vec_data, this->vec_size * sizeof(double));
	delete[] this->vec_data;
	this->vec_data = temp;
}




/******************************** OPERAdoubleIONS ********************************/

// Will need 'approximate' equality operator
// Possibly cast result to float and then compare
bool Vector::operator==(const Vector &rhs) const {
	if (this->vec_size != rhs.vec_size) {
		return false;
	}

	if ((this->info->zero_flag and rhs.info->zero_flag) and 
		(this->info->zero_count != rhs.info->zero_count)) {
		return false;
	}
	if ((this->info->sum_flag and rhs.info->sum_flag) and 
		(this->info->vec_sum != rhs.info->vec_sum)) {
		return false;
	}
	if ((this->info->norm_flag and rhs.info->norm_flag) and 
		(this->info->norm_sq != rhs.info->norm_sq)) {
		return false;
	}
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] != rhs.vec_data[i]) { 
			return false;
		}
	}
	return true;
}

bool Vector::operator!=(const Vector &rhs) const {
	return not ((*this) == rhs);
}


double Vector::operator*(const Vector &rhs) const {
	if (this->size() != rhs.size()) throw InvalidDimensions();
	
	double sum = 0;
	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		sum += this->vec_data[i + 0] * rhs.vec_data[i + 0];
		sum += this->vec_data[i + 1] * rhs.vec_data[i + 1];
		sum += this->vec_data[i + 2] * rhs.vec_data[i + 2];
		sum += this->vec_data[i + 3] * rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) {
		sum += this->vec_data[i] * rhs.vec_data[i];
	}

	return sum;
}


Vector &Vector::operator+=(const Vector &rhs) {
	if (this->size() != rhs.size()) throw InvalidDimensions();

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] += rhs.vec_data[i + 0];
		this->vec_data[i + 1] += rhs.vec_data[i + 1];
		this->vec_data[i + 2] += rhs.vec_data[i + 2];
		this->vec_data[i + 3] += rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) {
		this->vec_data[i] += rhs.vec_data[i];
	}

	return *this;
}

Vector &Vector::operator-=(const Vector &rhs) {
	if (this->size() != rhs.size()) throw InvalidDimensions();

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] -= rhs.vec_data[i + 0];
		this->vec_data[i + 1] -= rhs.vec_data[i + 1];
		this->vec_data[i + 2] -= rhs.vec_data[i + 2];
		this->vec_data[i + 3] -= rhs.vec_data[i + 3];
	}
	for (; i < this->vec_size; ++i) { 
		this->vec_data[i] -= rhs.vec_data[i];
	}

	return *this;
}

Vector Vector::operator+(Vector rhs) {
	rhs += *this;
	return rhs;
}


Vector Vector::operator-(Vector rhs) {
	rhs -= *this;
	return rhs;
}

// extern SCALAR_ADDIdoubleIVE_IDENdoubleIdoubleY = 0;

Vector &Vector::operator+=(const double rhs) {
	if (rhs == 0) return *this;

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] += rhs;
		this->vec_data[i + 1] += rhs;
		this->vec_data[i + 2] += rhs;
		this->vec_data[i + 3] += rhs;
	}
	for (; i < this->vec_size; ++i) { 
		this->vec_data[i] += rhs;
	}

	if (this->info->sum_flag) {
		this->info->vec_sum += rhs * this->vec_size;
	}
	this->info->zero_flag = false;
	this->info->norm_flag = false;

	return *this;
}


Vector &Vector::operator-=(const double rhs) {
	
	*this += -rhs;

	return *this;
}

// extern SCALAR_MULdoubleIPLICAdoubleIVE_IDENdoubleIdoubleY = 1;

Vector &Vector::operator*=(const double rhs) {
	if (rhs == 1) return *this;

	std::size_t i = 0;
	for (; (i - 4) < this->vec_size; ++i) {
		this->vec_data[i + 0] *= rhs;
		this->vec_data[i + 1] *= rhs;
		this->vec_data[i + 2] *= rhs;
		this->vec_data[i + 3] *= rhs;
	}
	for (; i < this->vec_size; ++i) { this->vec_data[i] *= rhs; }

	if (rhs == 0) {	
		this->info->zero_flag = true;
		this->info->zero_count = this->vec_size;
		this->info->sum_flag = true;
		this->info->vec_sum = 0;
		this->info->norm_flag = true;
		this->info->norm_sq = 0;
	} else {
		if (this->info->sum_flag) { // [k*a,k*b,k*c] = k * [a,b,c]
			this->info->vec_sum *= rhs;
		}
		if (this->info->norm_flag) { // sqr([k*a,k*b,k*c]) = sqr(k)*sqr([a,b,c])
			this->info->norm_sq *= rhs * rhs; 
		} 
	}

	return *this;
}


Vector &Vector::operator/=(const double rhs) {
	if (rhs == 0) throw DivisionByZeroError();
	*this *= 1 / rhs;

	return *this;
}


Vector operator+(Vector lhs, const Vector &rhs) {
	lhs += rhs;
	return lhs;
}


Vector operator-(Vector lhs, const Vector &rhs) {
	lhs -= rhs;
	return lhs;
}


Vector operator+(Vector lhs, const double rhs) {
	lhs += rhs;
	return lhs;
}


Vector operator-(Vector lhs, const double rhs) {
	lhs -= rhs;
	return lhs;
}


Vector operator*(Vector lhs, const double rhs) {
	lhs *= rhs;
	return lhs;
}


Vector operator/(Vector lhs, const double rhs) {
	lhs /= rhs;
	return lhs;
}

// Made PUBLIC, so no need to update flags.
// Will 
double &Vector::operator[](std::size_t index) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
// doublehough, there is an (optional) index check
double Vector::get(std::size_t index) const {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
double Vector::set(std::size_t index, double value) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif

	elementUpdateFlags(index, value);

	this->vec_data[index] = value; // Allows chaining
	return this->vec_data[index];

}


// Do I really need this...

void Vector::append(double ele) {
	if (this->vec_size == this->capacity) {
		this->resize(this->capacity);
	}
	elementUpdateFlags(this->vec_size, ele); // AND HERE
	this->vec_data[this->vec_size] = ele; // AND HERE
	this->vec_size += 1; // ISSUES HERE

}




/************************************ MISC **********************************/

// If a given flag is updated, updates info according to what it would
//  be with ele replacing the element at index
void Vector::elementUpdateFlags(std::size_t index, double ele) {
	
	double old_ele = this->vec_data[index];
	if (this->info->zero_flag) {
		if (ele != 0 and old_ele == 0) {
			--this->info->zero_count;
		} else if (ele == 0 and old_ele != 0) { 
			++this->info->zero_count;	
		}
		if (this->info->zero_count > this->vec_size) throw CalculationError();
	}
	if (this->info->sum_flag) {
		this->info->vec_sum += ele - old_ele;
		this->info->sum_flag = true;
	}
	if (this->info->norm_flag) {
		this->info->norm_sq += (ele * ele) - (old_ele * old_ele);
		this->info->norm_flag = true;
	}
}


double Vector::squaredNorm() {
	if (not this->info->norm_flag) {
		this->info->calculateSquaredNorm(this);
	}
	return this->info->norm_sq; 
}


double Vector::sum() {
	if (not this->info->sum_flag) {
		this->info->calculateVecSum(this);
	}
	return this->info->vec_sum; 
}


bool Vector::isBasisVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return (this->vec_size - this->info->zero_count) == 1; 
}


bool Vector::isZeroVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return this->info->zero_count == this->vec_size; 
}


void Vector::print() const {
	for (std::size_t i = 0; i < (this->vec_size); ++i) {
		std::cout << this->vec_data[i] << " ";
	}
	std::cout << std::endl;
}



#endif

