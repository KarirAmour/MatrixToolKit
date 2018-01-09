#include "vector.h"

#include <cstring>
#include <memory>
#include <iostream>



/****************************************************************************\
|********************************** BIG FIVE ********************************|
\****************************************************************************/

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


template <typename T>
void swap(Vector<T> &first, Vector<T> &second) {
	using std::swap;

	swap(first.vec_size, second.vec_size);
	swap(first.capacity, second.capacity);
	swap(first.vec_data, second.vec_data);
	swap(first.info, second.info);

}
// Copy Assigment

template <typename T>
Vector &Vector<T>::operator=(Vector other) {

	swap(*this, other);
	return *this;
}

template <typename T>
Vector<T>::~Vector() {
	delete this->info;
	delete[] this->vec_data;
}


/****************************************************************************\
|****************************** DATA ALLOCATION *****************************|
\****************************************************************************/

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
// Enable ability to decrease size?

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



/****************************************************************************\
|******************************** OPERATIONS ********************************|
\****************************************************************************/


// Will need 'approximate' equality operator
// Possibly cast result to float and then compare

template <typename T>
bool Vector<T>::operator==(const Vector &rhs) const {
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
bool Vector<T>::operator!=(const Vector &rhs) const {
	return not ((*this) == rhs);
}


template <typename T>
T Vector<T>::operator*(const Vector &rhs) const {
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
Vector<T> &Vector<T>::operator+=(const Vector &rhs) {
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
Vector<T> &Vector<T>::operator-=(const Vector &rhs) {
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
Vector<T> operator+(Vector lhs, const Vector &rhs) {
	lhs += rhs;
	return lhs;
}


template <typename T>
Vector<T> operator-(Vector lhs, const Vector &rhs) {
	lhs -= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator+(Vector lhs, const T rhs) {
	lhs += rhs;
	return lhs;
}


template <typename T>
Vector<T> operator-(Vector lhs, const T rhs) {
	lhs -= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator*(Vector lhs, const T rhs) {
	lhs *= rhs;
	return lhs;
}


template <typename T>
Vector<T> operator/(Vector lhs, const T rhs) {
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




/****************************************************************************\
|************************************ MISC **********************************|
\****************************************************************************/

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




