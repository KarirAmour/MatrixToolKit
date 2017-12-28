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
	Matrix(const Matrix &other);
	Matrix(Matrix &&other);
	// Matrix &operator=(const Matrix &other);
	// Matrix &operator=(const Matrix &&other);
	~Matrix();


	void allocateData();
	void print() const;
	void readCSV(const char *file_name);

	bool operator==(const Matrix& rhs) const;
	std::size_t getCols() const { return this->num_cols; }
	std::size_t getRows() const { return this->num_rows; }

    // Matrix operator*(Matrix const& rhs) const;
	Matrix naiveMultiply(const Matrix &rhs) const;
	Matrix naiveLoopMultiply(const Matrix &rhs) const;

    // TYPE dot(const Matrix &lhs, const Matrix &rhs, std::size_t row, std::size_t col) const;
	// Matrix operator+ (const Matrix& c) const;

	// void WriteTo(std::string file);
	// int my_atoi(const char *str);

	// void readCSV(std::istream &input);


};




#endif
