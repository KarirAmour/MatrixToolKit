#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cstddef>
#include <string>

#define TYPE int
#define BIT_SIZE 8
#define SEP (",")

class Matrix {
	std::size_t num_rows;
	std::size_t num_cols;
	// bool is_sparse;

	TYPE **data;

public:

	// Matrix(unsigned int rows, unsigned int cols);
	Matrix(const char *file_name, std::size_t nrows, std::size_t ncols);
	Matrix(std::size_t nrows, std::size_t nclos);
	Matrix(const Matrix &other); // Copy Constructor
	Matrix(Matrix &&other); // Move Constructor
	Matrix &operator=(Matrix other); // Copy Assigment
	Matrix &operator=(Matrix &&other); // Move Assignment
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
	// Matrix naiveMultiply(const Matrix &rhs) const;
	// Matrix naiveLoopMultiply(const Matrix &rhs) const;

    // TYPE dot(const Matrix &lhs, const Matrix &rhs, std::size_t row, std::size_t col) const;
	// Matrix operator+ (const Matrix& c) const;

	// void WriteTo(std::string file);
	// int my_atoi(const char *str);

	// void readCSV(std::istream &input);

	friend void swap(Matrix &first, Matrix &second);

};




#endif
