#ifndef _VECTOR_H_
#define _VECTOR_H_

// #include "vectorinfo.h"
#include "info.h"
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define INIT_SIZE 32
#define CHECK_BOUNDS 1


template <typename T>
class Vector {
		
	template<typename L>
	struct VectorInfo {

	/*********** FLAGS *********/
		bool zero_flag;
		bool sum_flag;
		bool norm_flag;

		std::size_t zero_count;
		L vec_sum;
		L norm_sq;
	/***************************/
		
		VectorInfo(bool zeroflg=false, bool sumflg=false, bool normflg=false, 
       	std::size_t zero=0, L sum=0.0, L norm=0.0) :
			zero_flag{zeroflg}, sum_flag{sumflg}, norm_flag{normflg}, 
			zero_count{zero}, vec_sum{sum}, norm_sq{norm} {}

		// All three methods use loop unrolling
		//  Lhough may be more efficient to create one method called updateAllInfo(),
		//   and loop through vector all in one   
		void countZeros(Vector<L> *vec) {

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
		void calculateVecSum(Vector<L> *vec) {

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
		void calculateSquaredNorm(Vector<L> *vec) {
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
		
	};


	/******* INFO *******/

	std::size_t vec_size;
	std::size_t capacity;

	T *vec_data;
	VectorInfo<T> *info;

	/***** METHODS ******/

	void allocateData(std::size_t amount);
	void resize(std::size_t amount);

	// Updates flags based on element TO BE added
	// If added element first, forced to check ENTIRE vector
	void elementUpdateFlags(std::size_t index, T ele);

	// get&set elements without needing to update flags at each access.
	T &operator[] (std::size_t index);
	void swap(Vector<T> &first, Vector<T> &second) noexcept;

public:

	Vector<T>(std::size_t length = 0);
	Vector<T>(std::size_t length, T init);
	Vector<T>(const Vector &vec);
	Vector<T>(std::vector<T> &vec);

	~Vector<T>();

	Vector<T> &operator=(Vector<T> other);
	bool operator==(const Vector<T> &rhs) const;
	bool operator!=(const Vector<T> &rhs) const;

	T operator*(const Vector<T> &rhs) const;

	Vector<T> &operator+=(const Vector<T> &rhs);
	Vector<T> &operator-=(const Vector<T> &rhs);
	Vector<T> operator+(Vector<T> &rhs);
	Vector<T> operator-(Vector<T> &rhs);
	
	Vector<T> &operator+=(const T rhs);
	Vector<T> &operator-=(const T rhs);
	Vector<T> &operator*=(const T rhs);
	Vector<T> &operator/=(const T rhs);


	T get(std::size_t index) const;
	T set(std::size_t index, T value);


	T sum();
	T squaredNorm();

	void append(T ele);

	bool isBasisVector();
	bool isZeroVector();

	void print() const;

	std::size_t getCapacity() const { return this->capacity; }
	std::size_t size() const { return this->vec_size; }
	
};




/******************************* CONSTRUCTORS ********************************/

template <typename T>
Vector<T>::Vector(std::size_t length) : 
		vec_size{length}, capacity{0}, info{new VectorInfo<T>()} {
	this->allocateData(length);
}


template <typename T>
Vector<T>::Vector(std::size_t length, T init) : vec_size{length}, capacity{0}, 
		info(new VectorInfo<T>(true, true, true, init == 0.0f ? length : 0.0f, 
							init*length, init*init*length)) {

	this->allocateData(length);
	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = init;
	}
}


template <typename T>
Vector<T>::Vector(std::vector<T> &vec) {

	this->info = new VectorInfo<T>();
	std::size_t length = vec.size();
	// std::cout << "Entered Vector(std::vector v) " << vec.size() << std::endl;
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
template <typename T>
Vector<T>::Vector(const Vector<T> &other) : 
	vec_size{other.vec_size}, capacity{other.capacity}, 
	info{new VectorInfo<T>(other.info)} {

	this->allocateData(this->vec_size);

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->vec_data[i] = other.vec_data[i];
	}
}

// Overloading swap to optimize copy and move assignment
template <typename T>
void Vector<T>::swap(Vector<T> &first, Vector<T> &second) noexcept {
	using std::swap;

	swap(first.vec_size, second.vec_size);
	swap(first.capacity, second.capacity);
	swap(first.vec_data, second.vec_data);
	swap(first.info, second.info);

}


// Copy Assigment
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> other) {

	swap(*this, other);
	return *this;
}

// Destructor
template <typename T>
Vector<T>::~Vector() {
	delete this->info;
	delete[] this->vec_data;
}


/****************************** DATA ALLOCATION *****************************/

// Should only be called in constructor, else resize.
template <typename T>
void Vector<T>::allocateData(std::size_t amount) {
	// std::cout << "allocData(" << amount <<")" << std::endl;

	// std::cout << nextPowerOfTwo(amount) << std::endl;
	this->vec_size = amount;
	this->capacity = amount ? nextPowerOfTwo(amount) : INIT_SIZE;
	this->vec_data = new T[this->capacity];
	// std::cout << "Exiting allocData" << std::endl;

}

// Can only increase size
// Should enable ability to decrease size?
template <typename T>
void Vector<T>::resize(std::size_t amount) {

	std::size_t new_capacity = nextPowerOfTwo(this->vec_size + amount);
	if (this->capacity >= new_capacity) { 
		return;
	}

	this->capacity = new_capacity;
	T *temp = new T[this->capacity];
	std::memcpy(temp, this->vec_data, this->vec_size * sizeof(T));
	delete[] this->vec_data;
	this->vec_data = temp;
}




/******************************** OPERATIONS ********************************/

// Will need 'approximate' equality operator
// Possibly cast result to float and then compare
template <typename T>
bool Vector<T>::operator==(const Vector<T> &rhs) const {
	if (this->vec_size != rhs.vec_size) {
		// std::cout << "size!=size" << std::endl;
		return false;
	}

	if ((this->info->zero_flag and rhs.info->zero_flag) and 
		(this->info->zero_count != rhs.info->zero_count)) {
		// std::cout << "cnt!=cnt" << std::endl;
		// std::cout <<  this->info->zero_count << " != " << rhs.info->zero_count << std::endl;
		return false;
	}
	if ((this->info->sum_flag and rhs.info->sum_flag) and 
		(this->info->vec_sum != rhs.info->vec_sum)) {
		// std::cout << "sum!=sum" << std::endl;
		// std::cout <<  this->info->vec_sum << " != " << rhs.info->vec_sum << std::endl;
		return false;
	}
	if ((this->info->norm_flag and rhs.info->norm_flag) and 
		(this->info->norm_sq != rhs.info->norm_sq)) {
		// std::cout << "norm!=norm" << std::endl;
		return false;
	}
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] != rhs.vec_data[i]) { 
			// std::cout << i << " != i" << std::endl;
			return false;
		}
	}
	return true;
}

template <typename T>
bool Vector<T>::operator!=(const Vector<T> &rhs) const {
	return not ((*this) == rhs);
}


template <typename T>
T Vector<T>::operator*(const Vector<T> &rhs) const {
	if (this->size() != rhs.size()) throw InvalidDimensions();
	
	T sum = 0;
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


template <typename T>
Vector<T> &Vector<T>::operator+=(const Vector<T> &rhs) {
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

template <typename T>
Vector<T> &Vector<T>::operator-=(const Vector<T> &rhs) {
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

template <typename T>
Vector<T> Vector<T>::operator+(Vector &rhs) {
	rhs += *this;
	return rhs;
}


template <typename T>
Vector<T> Vector<T>::operator-(Vector &rhs) {
	rhs -= *this;
	return rhs;
}

// extern SCALAR_ADDITIVE_IDENTITY = 0;

template <typename T>
Vector<T> &Vector<T>::operator+=(const T rhs) {
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


template <typename T>
Vector<T> &Vector<T>::operator-=(const T rhs) {
	
	*this += -rhs;

	return *this;
}

// extern SCALAR_MULTIPLICATIVE_IDENTITY = 1;

template <typename T>
Vector<T> &Vector<T>::operator*=(const T rhs) {
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


template <typename T>
Vector<T> &Vector<T>::operator/=(const T rhs) {
	if (rhs == 0) throw DivisionByZeroError();
	*this *= 1 / rhs;

	return *this;
}


template <typename T>
Vector<T> operator+(Vector<T> lhs, const Vector<T> &rhs) {
	lhs += rhs;
	return lhs;
}


template <typename T>
Vector<T> operator-(Vector<T> lhs, const Vector<T> &rhs) {
	lhs -= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator+(Vector<T> lhs, const T rhs) {
	lhs += rhs;
	return lhs;
}


template <typename T>
Vector<T> operator-(Vector<T> lhs, const T rhs) {
	lhs -= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator*(Vector<T> lhs, const T rhs) {
	lhs *= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator/(Vector<T> lhs, const T rhs) {
	lhs /= rhs;
	return lhs;
}

// Made private, so no need to update flags.
template <typename T>
T &Vector<T>::operator[](std::size_t index) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
// Though, there is an (optional) index check
template <typename T>
T Vector<T>::get(std::size_t index) const {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
template <typename T>
T Vector<T>::set(std::size_t index, T value) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif

	elementUpdateFlags(index, value);

	this->vec_data[index] = value; // Allows chaining
	return this->vec_data[index];

}


// Do I really need this...

template <typename T>
void Vector<T>::append(T ele) {
	if (this->vec_size == this->capacity) {
		this->resize(this->capacity);
	}
	elementUpdateFlags(this->vec_size, ele); // AND HERE
	this->vec_data[this->vec_size] = ele; // AND HERE
	this->vec_size += 1; // ISSUES HERE

}




/************************************ MISC **********************************/

// void Vector<T>::updateFlags() {
// 	this->info->countZeros();
// 	this->info->calculateVecSum();
// 	this->info->calculateSquaredNorm();
// }

// If a given flag is updated, updates info according to what it would
//  be with ele replacing the element at index
template <typename T>
void Vector<T>::elementUpdateFlags(std::size_t index, T ele) {
	
	T old_ele = this->vec_data[index];
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


template <typename T>
T Vector<T>::squaredNorm() {
	if (not this->info->norm_flag) {
		this->info->calculateSquaredNorm(this);
	}
	return this->info->norm_sq; 
}


template <typename T>
T Vector<T>::sum() {
	if (not this->info->sum_flag) {
		this->info->calculateVecSum(this);
	}
	return this->info->vec_sum; 
}


template <typename T>
bool Vector<T>::isBasisVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return (this->vec_size - this->info->zero_count) == 1; 
}


template <typename T>
bool Vector<T>::isZeroVector() {
	if (not this->info->zero_flag) {
		this->info->countZeros(this);
	}
	return this->info->zero_count == this->vec_size; 
}


template <typename T>
void Vector<T>::print() const {
	// std::cout << this->vec_size << " " << this->size() << std::endl;
	for (std::size_t i = 0; i < (this->vec_size); ++i) {
		std::cout << this->vec_data[i] << " ";
	}
	std::cout << std::endl;
}





#endif

