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

Matrix::Matrix(std::size_t num_rows, std::size_t num_cols) : 
	num_rows{num_rows}, num_cols{num_cols} {

		this->allocateData();


}

void Matrix::allocateData() {
	// Allocate enough data
	this->data = new TYPE *[this->num_rows];
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		this->data[row] = new TYPE[this->num_cols];
		// if (not this->data[row]) throw NoMemory();
	}
}


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


void Matrix::readCSV(const char *file_name) {

	clock_t t;
	t = clock();

	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("No such file.\n");
		exit(1);
	} else {
		printf("Opened file.\n");
	}

	int buff_size = (this->num_cols * sizeof(TYPE) + 1) * 8;
	for (std::size_t n_row = 0; n_row < this->num_rows; ++n_row) {
		char * row_data = new char[buff_size];
		fgets(row_data, buff_size, fp);
		char *token;
		token = std::strtok(row_data, SEP);
		// std::cout << n_row << std::endl;
		// printf("row %d: %s\n", n_row, row_data);
		for (std::size_t n_col = 0; n_col < this->num_cols; ++n_col) {
			// std::cout << n_col << ": " << atoi(token) << ", " << token << std::endl;

			// printf("data[%d][%d] = %s\n", n_row, n_col, token);
			this->data[n_row][n_col] = atoi(token);

			token = std::strtok(NULL, SEP);
		}
		delete[] row_data;
	}

	fclose(fp);




	t = clock() - t;
	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}

void Matrix::print() {
	for (std::size_t row = 0; row < num_rows; ++row) {
		for (std::size_t col = 0; col < num_cols; ++col) {
			std::cout << this->data[row][col] << " ";
		}
		std::cout << std::endl;
	}

}


// Matrix Matrix::operator+ (const Matrix& c) const {

// }
