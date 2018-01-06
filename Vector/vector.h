#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "info.h"
#include "vectorinfo.h"
#include <cstdlib>
#include <vector>
#include <memory>

#define INIT_SIZE 32
#define CHECK_BOUNDS 1



class Vector {
	
	friend class VectorInfo;
	
	/******* INFO *******/

	std::size_t vec_size;
	std::size_t capacity;

	TYPE *vec_data;
	VectorInfo *info;

	/***** METHODS ******/

	void allocateData(std::size_t amount);
	void resize(std::size_t amount);

	// Updates flags based on element TO BE added
	// If added element first, forced to check ENTIRE vector
	void elementUpdateFlags(std::size_t index, TYPE ele);

	// get&set elements without needing to update flags at each access.
	TYPE &operator[] (std::size_t index);

public:

	Vector(std::size_t length = 0);
	Vector(std::size_t length, TYPE init);
	Vector(const Vector &vec);
	Vector(std::vector<TYPE> &vec);

	~Vector();

	Vector &operator=(Vector other);
	bool operator==(const Vector &rhs) const;
	bool operator!=(const Vector &rhs) const;

	TYPE operator*(const Vector &rhs) const;
	
	Vector &operator+=(const Vector &rhs);
	Vector &operator-=(const Vector &rhs);
	
	Vector &operator+=(const TYPE rhs);
	Vector &operator-=(const TYPE rhs);
	Vector &operator*=(const TYPE rhs);
	Vector &operator/=(const TYPE rhs);


	TYPE get(std::size_t index) const;
	TYPE set(std::size_t index, TYPE value);


	TYPE sum();
	TYPE squaredNorm();

	void append(TYPE ele);

	bool isBasisVector();
	bool isZeroVector();

	void print() const;

	std::size_t getCapacity() const { return this->capacity; }
	std::size_t size() const { return this->vec_size; }

	friend void swap(Vector &first, Vector &second);
	
};

Vector operator+(Vector lhs, const Vector &rhs);
Vector operator-(Vector lhs, const Vector &rhs);
Vector operator*(Vector lhs, const Vector &rhs);
Vector operator/(Vector lhs, const Vector &rhs);



#endif

