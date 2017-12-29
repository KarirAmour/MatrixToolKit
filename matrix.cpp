#include "matrix.h"
// #include <cstdlib>
// #include <sstream>
// #include <fstream>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstring>


class FileNotFound {};
class NoMemory {};
class InvalidRow {};
class InvalidCol {};
class InvalidRead {};
class InvalidDimensions {};

// int Matrix::my_atoi(const char *str) {
//     while (*str == ' ') ++str; // skip leading whitespace
//     int sign = 1;
//     switch (*str) { // handle leading +/-
//         case '-': ++str; sign = -1; break;
//         case '+': ++str; break;
//     }
//     int value = 0;
//     while (char c = *str) {
//         if (c > '9' || value < 0) return 0; // non-digit, or overflow
//         value *= 10;
//         value += c - '0';
//     }
//     if (value < 0) return 0; // integer overflow
//     return value * sign;
// }

Matrix::Matrix(const char *file_name, std::size_t num_rows, std::size_t num_cols) : 
	num_rows{num_rows}, num_cols{num_cols} {

		this->allocateData();

		// std::fstream file (file_name, std::fstream::in);
		// if (! file.is_open()) throw FileNotFound();

		this->readCSV(file_name);

}

Matrix::Matrix(std::size_t num_rows = 0, std::size_t num_cols = 0) : 
	num_rows{num_rows}, num_cols{num_cols}, data{NULL} {

		this->allocateData();


}

void Matrix::allocateData() {
	// Allocate enough data
	if (this->num_cols == 0 or this->num_rows == 0) return;

	this->data = new TYPE *[this->num_rows];
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		this->data[row] = new TYPE[this->num_cols];
		// if (not this->data[row]) throw NoMemory();
	}
}

// Destructor
Matrix::~Matrix() {
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		delete[] this->data[row];
	}
	delete[] this->data;
	std::cout << "Destructor" << std::endl;

}

// Copy Constructor
Matrix::Matrix(const Matrix &other) : num_rows{other.num_rows}, num_cols{other.num_cols} {

	// clock_t t;
	// t = clock();
	this->allocateData();
	for (std::size_t row = 0; row < num_rows; ++row) {
		std::copy(other.data[row], other.data[row] + num_rows, this->data[row]);
	}

	
	// t = clock() - t;
	// std::cout << "Copy ctor: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
	std::cout << "Copy Constructor" << std::endl;

}


// Move Constructor
Matrix::Matrix(Matrix &&other) : num_rows{other.num_rows}, num_cols{other.num_cols} {
	this->data = other.data;
	other.num_cols = other.num_rows = 0;
	other.data = NULL;
	std::cout << "Move Constructor" << std::endl;
}

void swap(Matrix &first, Matrix &second) {
	// enable ADL (not necessary in our case, but good practice)
	using std::swap;

	swap(first.num_rows, second.num_rows);
	swap(first.num_cols, second.num_cols);
	swap(first.data, second.data);

}

// Copy Assignment
Matrix &Matrix::operator=(Matrix other) {

	swap(*this, other);

	std::cout << "Copy Assignment" << std::endl;

	return *this;

}

// Move Assignment
Matrix &Matrix::operator=(Matrix &&other) {
	swap(*this, other);
	std::cout << "Move Assignment" << std::endl;
	return *this;

}



bool Matrix::operator==(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) {
		return false;
	}
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			if (this->data[row][col] != rhs.data[row][col]) {
				return false;
			}
		}
	}
	return true;
}



void Matrix::readCSV(const char *file_name) {

	// clock_t t;
	// t = clock();

	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		// printf("No such file.\n");
		exit(1);
	} else {
		// printf("Opened file.\n");
	}

	int buff_size = (this->num_cols * sizeof(TYPE) + 1) * 8;
	for (std::size_t n_row = 0; n_row < this->num_rows; ++n_row) {
		char * row_data = new char[buff_size];
		if (not fgets(row_data, buff_size, fp)) throw InvalidRead();
		char *token;
		token = std::strtok(row_data, SEP);
		for (std::size_t n_col = 0; n_col < this->num_cols; ++n_col) {
			this->data[n_row][n_col] = atoi(token);
			token = std::strtok(NULL, SEP);
		}
		delete[] row_data;
	}

	fclose(fp);




	// t = clock() - t;
	// std::cout << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
}

void Matrix::print() const {
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			std::cout << this->data[row][col] << " ";
		}
		std::cout << std::endl;
	}

}



Matrix Matrix::operator*(const Matrix &rhs) const {
	// clock_t t = clock();

	if (this->num_cols != rhs.num_rows) throw InvalidDimensions();

	Matrix product = Matrix(this->num_rows, rhs.num_cols);
	for (std::size_t row = 0; row < product.num_rows; ++row) {
		for (std::size_t col = 0; col < product.num_cols; ++col) {
			TYPE sum = 0;
			for (std::size_t k = 0; k < this->num_cols; ++k) {
				sum += this->data[row][k] * rhs.data[k][col];

			}
			product.data[row][col] = sum;
		}

	}

	// t = clock() - t;
	// std::cout << "naiveMultiply: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	return product;

}


Matrix Matrix::operator*(TYPE scalar) const {
	
	Matrix product(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			product.data[row][col] *= scalar;
		}	
	}
	return product;
}



Matrix Matrix::operator+(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) throw InvalidDimensions();
	
	Matrix sum(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			sum.data[row][col] += rhs.data[row][col];
		}	
	}
	return sum;
}

Matrix Matrix::operator+(TYPE scalar) const {
	
	Matrix sum(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			sum.data[row][col] += scalar;
		}	
	}
	return sum;
}




Matrix Matrix::operator-(TYPE scalar) const {
	return Matrix(*this + (-scalar));
}


Matrix Matrix::Matrix::operator-(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) throw InvalidDimensions();
	
	Matrix difference(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			difference.data[row][col] -= rhs.data[row][col];
		}	
	}
	return difference;
}



bool Matrix::operator!=(const Matrix &rhs) const {
	return not (*this == rhs);
}
























// TYPE Matrix::dot(const Matrix &lhs, const Matrix &rhs, std::size_t row, std::size_t col) const {
// 	int sum = 0;
// 	if (lhs.num_cols != rhs.num_rows) throw InvalidDimensions();

// 	for (std::size_t loop = 0; loop < lhs.num_cols; ++loop) {
// 		sum += lhs.data[row][loop] * rhs.data[loop][col];
// 	}

// 	return sum;
// }


// Matrix Matrix::naiveMultiply(const Matrix &rhs) const {

// 	// clock_t t = clock();

// 	if (this->num_cols != rhs.num_rows) throw InvalidDimensions();

// 	Matrix product = Matrix(this->num_rows, rhs.num_cols);
// 	for (std::size_t row = 0; row < product.num_rows; ++row) {
// 		for (std::size_t col = 0; col < product.num_cols; ++col) {
// 			TYPE sum = 0;
// 			for (std::size_t k = 0; k < this->num_cols; ++k) {
// 				sum += this->data[row][k] * rhs.data[k][col];

// 			}
// 			product.data[row][col] = sum;
// 		}

// 	}

// 	// t = clock() - t;
// 	// std::cout << "naiveMultiply: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

// 	return product;

// }


// Matrix Matrix::naiveLoopMultiply(const Matrix &rhs) const {

// 	// clock_t t = clock();

// 	if (this->num_cols != rhs.num_rows) throw InvalidDimensions();

// 	Matrix product = Matrix(this->num_rows, rhs.num_cols);
// 	for (std::size_t row = 0; row < product.num_rows; row += 2) {
// 		for (std::size_t col = 0; col < product.num_cols; col += 2) {
// 			TYPE acc00 = 0, acc01 = 0, acc10 = 0, acc11 = 0;
// 			for (std::size_t k = 0; k < this->num_cols; ++k) {
// 				acc00 += this->data[k][col + 0] * rhs.data[row + 0][k];
// 				acc01 += this->data[k][col + 1] * rhs.data[row + 0][k];
// 				acc10 += this->data[k][col + 0] * rhs.data[row + 1][k];
// 				acc11 += this->data[k][col + 1] * rhs.data[row + 1][k];
// 			}
// 			product.data[row + 0][col + 0] = acc00;
// 			product.data[row + 0][col + 1] = acc01;
// 			product.data[row + 1][col + 0] = acc10;
// 			product.data[row + 1][col + 1] = acc11;
// 		}
// 	}
// 	// For last column if product.n_cols is odd
// 	if (product.num_cols % 2 == 1) {
// 		for (std::size_t row = 0; row < this->num_cols; ++row) {
// 			TYPE sum = 0;
// 			for (std::size_t k = 0; k < this->num_cols; ++k) {
// 				sum += this->data[row][k] * rhs.data[k][product.num_cols - 1];
// 			}
// 			product.data[row][col] = sum;
// 		}
// 	}



// 	// t = clock() - t;
// 	// std::cout << "naiveLoopMultiply: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

// 	return product;

// }



// Matrix Matrix::operator+(const Matrix& c) const {
// 	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) {
// 		throw InvalidDimensions();
// 	}

// 	Matrix sum = Matrix(this->num_rows, this->num_cols);
// 	for (std::size_t row = 0; row < product.num_rows; row += 2) {
// 		for (std::size_t col = 0; col < product.num_cols; col += 2) {
// }





// void Matrix::readCSV(std::istream &input) {

// 	clock_t t;
// 	t = clock();

// 	std::string line;
// 	size_t row = 0;

// 	while ( getline(input, line) ) {

// 		std::istringstream ss(line);
// 		TYPE cell = 0;
// 		size_t col = 0;
// 		while (ss.good()) {
// 			std::string cellString;
// 		    getline( ss, cellString, ',' );
// 		    std::istringstream toIntSS(cellString);
// 		    toIntSS >> cell;
// 		    if (col >= num_cols) throw InvalidCol();
// 			this->data[row][col++] = cell;
// 			// std::cout << row << ", " << col << std::endl;
// 		}
		
// 		if (row >= num_rows) throw InvalidCol();
// 		++row;

// 	}
// 	t = clock() - t;
// 	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
// }
