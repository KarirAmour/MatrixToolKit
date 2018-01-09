#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "info.h"
#include "vectorinfo.h"
#include <cstdlib>
#include <vector>
#include <memory>

#define INIT_SIZE 32
#define CHECK_BOUNDS 1


template <typename T>
class Vector {
	
	friend class VectorInfo<T>;
	
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

public:

	Vector(std::size_t length = 0);
	Vector(std::size_t length, T init);
	Vector(const Vector &vec);
	Vector(std::vector<T> &vec);

	~Vector();

	Vector &operator=(Vector other);
	bool operator==(const Vector &rhs) const;
	bool operator!=(const Vector &rhs) const;

	T operator*(const Vector &rhs) const;

	Vector &operator+=(const Vector &rhs);
	Vector &operator-=(const Vector &rhs);
	Vector operator+(Vector &rhs);
	Vector operator-(Vector &rhs);
	
	Vector &operator+=(const T rhs);
	Vector &operator-=(const T rhs);
	Vector &operator*=(const T rhs);
	Vector &operator/=(const T rhs);


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

	friend void swap(Vector &first, Vector &second);
	
};

template<typename T> Vector<T> operator+(Vector<T> lhs, const T rhs);
template<typename T> Vector<T> operator-(Vector<T> lhs, const T rhs);
template<typename T> Vector<T> operator*(Vector<T> lhs, const T rhs);
template<typename T> Vector<T> operator/(Vector<T> lhs, const T rhs);



#endif

