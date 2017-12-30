#include "vector.h"

#include <cstring>
#include <iostream>




/****************************************************************************\
|********************************** BIG FIVE ********************************|
\****************************************************************************/


Vector::Vector(std::size_t length) : vec_size{length}, capacity{0},
flags_valid{false}, is_zero{false}, is_basis{false}, sum_flag{false}, 
norm_sq_flag{false}, vec_sum{0}, norm_sq{0}, zero_count{0} {

	std::cout << "Vector size " << sizeof(*this) << std::endl;
	this->allocateData(length);
}

Vector::Vector(std::size_t length, TYPE init) : vec_size{length}, capacity{0}, 
flags_valid{true}, is_zero{init == 0}, is_basis{length == 1}, sum_flag{true}, 
norm_sq_flag{true}, vec_sum{init * length}, norm_sq{init * init * length}, zero_count{init == 0 ? length : 0} {

	this->allocateData(length);
	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = init;
	}
}


Vector::Vector(std::vector<TYPE> &vec) : flags_valid{false}, is_zero{false}, is_basis{false} {

	std::size_t length = vec.size();
	this->allocateData(length);

	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = vec[i];
		this->vec_sum += this->vec_data[i];
		this->norm_sq += this->vec_data[i] * this->vec_data[i];
	}
	this->sum_flag = true;
	this->norm_sq_flag = true;
}

// Copy Constructor

Vector::Vector(const Vector &other) : vec_size{other.vec_size}, capacity{other.capacity}, 
flags_valid{other.flags_valid}, is_zero{other.is_zero}, is_basis{other.is_basis}, 
sum_flag{other.sum_flag}, norm_sq_flag{other.norm_sq_flag}, 
vec_sum{other.vec_sum}, norm_sq{other.norm_sq}, zero_count{zero_count} {

	this->allocateData(this->vec_size);

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->vec_data[i] = other.vec_data[i];
	}
}


// // Move Constructor
// Vector::Vector(Vector &&other) {

// }

// // Copy Assigment
// Vector &Vector::operator=(Vector other) {

// }

Vector::~Vector() {
	delete[] this->vec_data;
}


/****************************************************************************\
|****************************** DATA ALLOCATION *****************************|
\****************************************************************************/

// Should only be called in constructor, else resize.
void Vector::allocateData(std::size_t amount) {

	this->vec_size = amount; // Nearest Divisor of INIT_SIZE. Can try nearest power of 2
	this->capacity = amount + INIT_SIZE - (amount % INIT_SIZE);
	this->vec_data = new TYPE[this->capacity];


}

// Can only increase size
void Vector::resize(std::size_t amount) {
	std::cout << "resize" << std::endl;
	TYPE *temp = new TYPE[this->capacity + amount + INIT_SIZE - (amount % INIT_SIZE)];
	std::memcpy(temp, vec_data, this->vec_size * sizeof(TYPE));
	this->capacity += amount;
	delete[] vec_data;
	vec_data = temp;
}



/****************************************************************************\
|******************************** OPERATIONS ********************************|
\****************************************************************************/

// Made private, so no need to update flags.
TYPE &Vector::operator[](std::size_t index) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
TYPE Vector::get(std::size_t index) const {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

// Returns value => no update to flags_valid => Faster than operator[]
TYPE Vector::set(std::size_t index, TYPE value) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif

	elementUpdateFlags(index, value);

	return this->vec_data[index] = value; // Allows chaining

}

bool Vector::operator==(const Vector &rhs) const {

	if (this->vec_size != rhs.vec_size) return false;
	if (this->is_zero != rhs.is_zero) return false;
	if (this->is_basis != rhs.is_basis) return false;

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] != rhs.vec_data[i]) return false;
	}
	return true;
}

bool Vector::operator!=(const Vector &rhs) const {
	return (*this) != rhs;
}


// Do I really need this...
void Vector::append(TYPE ele) {
	if (this->vec_size == this->capacity) {
		this->resize(this->capacity);
	}
	this->vec_data[this->vec_size] = ele;
	this->vec_size += 1;

	elementUpdateFlags(this->vec_size, ele);
}

void Vector::elementUpdateFlags(std::size_t index, TYPE ele) {
	if (this->flags_valid) {
		if (ele != 0) {
			if (this->is_zero) {
				this->is_zero = false;
				if (ele == 1) this->is_basis = true;
			} else {
				this->is_basis = false;
			}
			this->vec_sum += ele;
			this->norm_sq += ele * ele;
		}
	}
}




/****************************************************************************\
|************************************ MISC **********************************|
\****************************************************************************/

void Vector::updateFlags() {
	this->calculateIsBasis();
	this->calculateIsZero();
	this->calculateVecSum();
	this->calculateSquaredNorm();
	this->flags_valid = true;
}


TYPE Vector::squaredNorm() {
	if (not this->flags_valid) {
		this->updateFlags();
	}
	return this->norm_sq; 
}

void Vector::Vector::calculateSquaredNorm() {
	this->norm_sq = 0;
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->norm_sq += this->vec_data[i] * this->vec_data[i];
	}
}


TYPE Vector::Sum() {
	if (not this->flags_valid) {
		this->updateFlags();
	}
	return this->vec_sum; 
}

void Vector::calculateVecSum() {
	this->vec_sum = 0;
	for (std::size_t i = 0; i < this->vec_size; ++i) this->vec_sum += this->vec_data[i];

}

bool Vector::isBasisVector() {
	if (not this->flags_valid) {
		this->updateFlags();
	}
	return this->is_basis; 
}

bool Vector::isZeroVector() {
	if (not this->flags_valid) {
		this->updateFlags();
	}
	return this->is_zero; 
}


void Vector::calculateIsBasis() {
	this->is_basis = false;
	bool foundOne = false;
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] == 1) {
			if (foundOne) return;
			else foundOne = true;
		} else if (this->vec_data[i] != 0) return;
	}
	this->is_basis = foundOne;
}


void Vector::calculateIsZero() {

	this->is_zero = false;
	std::size_t i = 0;
	for (i = 0; (i - 4) < this->vec_size; ++i) {
		if (this->vec_data[i + 0]) return;
		if (this->vec_data[i + 1]) return;
		if (this->vec_data[i + 2]) return;
		if (this->vec_data[i + 3]) return;
	}

	for (; i < this->vec_size; ++i) if (this->vec_data[i]) return;

	this->is_zero = true;
}

void Vector::print() const {
	for (std::size_t i = 0; i < (this->vec_size); ++i) {
		std::cout << this->vec_data[i] << " ";
	}
	std::cout << std::endl;
}


// static void Test() {


// }