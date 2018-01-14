#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "info.h"
#include "../Vector/vector.h"
#include <cstddef>
#include <vector>
#include <memory>
// template <typename> class std::vector<double>;
// typedef Vector<double> XVector;
// typedef Vector<double>* PVector;
typedef Vector<double>* VectorPtr;
typedef Vector<double> VectorX;

struct MatrixInfo {

	bool validState; // set false when matrix is changed.
	bool isUpper;
	bool isLower;
	bool isZero;
	bool isIdentity;

	MatrixInfo() : validState{false}, isUpper{false}, isLower{false}, isZero{false}, isIdentity{false} {}
};

class Matrix {

	std::size_t num_rows;
	std::size_t num_cols;

	std::vector< std::shared_ptr<VectorX> > data;

	void determineInfo();

	void setIsUpper();
	void setIsLower();
	void setIsZero();
	void setIsIdentity();
	void revalidateState();
	
public:

	// CONNSIDER named constructor idiom (make ctors private 
	// and add static methods that call constructors
	// static ZeroMatrix(std::size_t nrows = 0, std::size_t ncols = 0);
	// static IdentityMatrix(std::size_t nrows = 0, std::size_t ncols = 0);
	// static OnesMatrix(std::size_t nrows = 0, std::size_t ncols = 0);
	// static PermutationMatrix(const std::vector<TYPE> &vec);
	// static InverseMatrix(const Matrix &m);

	std::shared_ptr<MatrixInfo> info;
	Matrix(const char *file_name);
	Matrix(std::size_t nrows = 0, std::size_t ncols = 0);
	Matrix(const Matrix &other); // Copy Constructor
	Matrix(Matrix &&other); // Move Constructor
	Matrix &operator=(Matrix other); // Copy Assigment
	~Matrix();


	void allocateData();
	void print() const;

	void readCSV(const char *file_name);

	bool operator==(const Matrix& rhs) const;
	bool operator!=(const Matrix &rhs) const;

	std::size_t getCols() const { return this->num_cols; }
	std::size_t getRows() const { return this->num_rows; }



    Matrix operator*(const Matrix &rhs) const;
	Matrix operator+(const Matrix &rhs) const;
	Matrix operator-(const Matrix &rhs) const;

	Matrix operator*(TYPE scalar) const;
	Matrix operator+(TYPE scalar) const;
	Matrix operator-(TYPE scalar) const;

	VectorX &operator[](std::size_t index);
	void permute(Vector<int> permutation);

	bool isUpperTriangular();
	bool isLowerTriangular();
	bool isZero();
	bool isIdentity();

	friend void swap(Matrix &first, Matrix &second);

	// Matrix &operator=(Matrix &&other); // Move Assignment (No need since copy/swap)
	// Matrix naiveMultiply(const Matrix &rhs) const;
	// Matrix naiveLoopMultiply(const Matrix &rhs) const;

    // TYPE dot(const Matrix &lhs, const Matrix &rhs, std::size_t row, std::size_t col) const;
	// Matrix operator+ (const Matrix& c) const;

	// void WriteTo(std::string file);
	// int my_atoi(const char *str);

	// void readCSV(std::istream &input);

};
// Don't declare static functions in header unless inlining the function
// static size_t csvRowCount(const char *file_name); 
// static size_t csvColumnCount(const char *file_name);



#endif
