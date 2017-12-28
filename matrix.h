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
	bool is_sparse;

	TYPE **data;

public:

	// Matrix(unsigned int rows, unsigned int cols);
	Matrix(const char *file_name, std::size_t nrows, std::size_t ncols);
	Matrix(std::size_t nrows, std::size_t ncols);
	void allocateData();
	// void WriteTo(std::string file);
	void print();
	// void readCSV(std::istream &input);
	void readCSV(const char *file_name);

	int my_atoi(const char *str);
	// Matrix operator+ (const Matrix& c) const;


};



#endif
