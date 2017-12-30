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

	bool flagsValid;
	bool isZero = false;
	bool isBasis = false;
	TYPE vecSum;
	TYPE normSq;

	TYPE *vec_data;

	void allocateData(std::size_t amount);
	void resize(std::size_t amount);
	void calculateVecSum();
	void calculateSquaredNorm();
	void calculateIsBasis();
	void calculateIsZero();
	void updateFlags();
public:

	Vector(std::size_t length);
	Vector(std::size_t length, TYPE init);
	Vector(std::vector<TYPE> &vec);
	~Vector();

	bool operator==(const Vector &rhs) const;
	bool operator!=(const Vector &rhs) const;
	TYPE &operator[] (std::size_t index);

	TYPE Sum();
	TYPE squaredNorm();

	void append(TYPE ele);

	bool isBasisVector();
	bool isZeroVector();

	void print() const;

	std::size_t getCapacity() const { return this->capacity; }
	std::size_t size() const { return this->vec_size; }

};

#endif
