#include "vector.h"

#include <cstring>
#include <iostream>


Vector::Vector(std::size_t length) {

	this->allocateData(length);
}

Vector::Vector(std::size_t length, TYPE init) {

	this->allocateData(length);

	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = init;
	}
}
Vector::~Vector() {
	delete[] vec_data;
}


Vector::Vector(std::vector<TYPE> &vec) {

	std::size_t length = vec.size();
	this->allocateData(length);

	for (std::size_t i = 0; i < length; ++i) {
		this->vec_data[i] = vec[i];
	}
}


void Vector::allocateData(std::size_t amount) {
	if (amount == 0) {
		this->vec_data = nullptr;
		this->vec_size = 0;
		this->capacity = 0;
		return;
	}
	this->vec_size = amount; // Nearest Divisor of INIT_SIZE. Can try nearest power of 2
	this->capacity = amount + INIT_SIZE - (amount % INIT_SIZE);
	this->vec_data = new TYPE[this->capacity];
}

// Need to change to return copy, else need to flagsValid is always false...
// add method this->set(ele); #NEED TO CHANGE
TYPE &Vector::operator[](std::size_t index) {
	#ifdef CHECK_BOUNDS
	if (index >= this->vec_size) throw OutOfBoundsException();
	#endif
	return this->vec_data[index];
}

bool Vector::operator==(const Vector &rhs) const {

	if (this->vec_size != rhs.vec_size) return false;
	if (this->isZero != rhs.isZero) return false;
	if (this->isBasis != rhs.isBasis) return false;

	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] != rhs.vec_data[i]) return false;
	}
	return true;
}

bool Vector::operator!=(const Vector &rhs) const {
	return (*this) != rhs;
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

// Do I really need this...
void Vector::append(TYPE ele) {
	if (this->vec_size == this->capacity) {
		this->resize(this->capacity);
	}
	this->vec_data[this->vec_size] = ele;
	this->vec_size += 1;
	this->vecSum += ele; // Dont need to update vecSum flag
	this->normSq += ele * ele; // Nor the normSq
	if (ele != 0) { // Can make more efficient by having individual flags? or related ones.
	if (this->isBasisVector() || this->isZeroVector()) this->flagsValid = false;
	}
}



void Vector::updateFlags() {
	this->calculateIsBasis();
	this->calculateIsZero();
	this->calculateVecSum();
	this->calculateSquaredNorm();
	this->flagsValid = true;
}


TYPE Vector::squaredNorm() {
	if (not this->flagsValid) {
		this->updateFlags();
	}
	return this->normSq; 
}

void Vector::calculateSquaredNorm() {
	this->normSq = 0;
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		this->normSq += this->vec_data[i] * this->vec_data[i];
	}
}


TYPE Vector::Sum() {
	if (not this->flagsValid) {
		this->updateFlags();
	}
	return this->vecSum; 
}

void Vector::calculateVecSum() {
	this->vecSum = 0;
	for (std::size_t i = 0; i < this->vec_size; ++i) this->vecSum += this->vec_data[i];

}

bool Vector::isBasisVector() {
	if (not this->flagsValid) {
		this->updateFlags();
	}
	return this->isBasis; 
}

bool Vector::isZeroVector() {
	if (not this->flagsValid) {
		this->updateFlags();
	}
	return this->isZero; 
}


void Vector::calculateIsBasis() {
	this->isBasis = false;
	bool foundOne = false;
	for (std::size_t i = 0; i < this->vec_size; ++i) {
		if (this->vec_data[i] == 1) {
			if (foundOne) return;
			else foundOne = true;
		} else if (this->vec_data[i] != 0) return;
	}
	this->isBasis = foundOne;
}


void Vector::calculateIsZero() {

	this->isZero = false;
	std::size_t i = 0;
	for (i = 0; (i - 4) < this->vec_size; ++i) {
		if (this->vec_data[i + 0]) return;
		if (this->vec_data[i + 1]) return;
		if (this->vec_data[i + 2]) return;
		if (this->vec_data[i + 3]) return;
	}

	for (; i < this->vec_size; ++i) if (this->vec_data[i]) return;

	this->isZero = true;
}

void Vector::print() const {
	for (std::size_t i = 0; i < (this->vec_size); ++i) {
		std::cout << this->vec_data[i] << " ";
	}
	std::cout << std::endl;
}


// static void Test() {


// }