#include "matrix.h"
#include "info.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>



/****************************************************************************\
|***********************************BIG FIVE*********************************|
\****************************************************************************/


Matrix::Matrix(const char *file_name) : num_rows{0}, num_cols{0} {


		this->readCSV(file_name);

		this->determineInfo();

}


void Matrix::determineInfo() {
	this->info = std::make_unique<MatrixInfo>(); // Thats it?
}

Matrix::Matrix(std::size_t nrows, std::size_t ncols) : 
	num_rows{nrows}, num_cols{ncols}, data{nullptr} {

		this->allocateData();
		this->determineInfo();

}

void Matrix::allocateData() {

	if (this->num_cols == 0 or this->num_rows == 0) {
		return;
	}

	this->data.reserve(this->num_rows);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		this->data.emplace_back(std::make_shared<VectorX>(this->num_cols));
	}
}

// Destructor
Matrix::~Matrix() {
	// for (std::size_t row = 0; row < this->num_rows; ++row) {
	// 	delete[] this->data[row];
	// }
	// delete[] this->data;
	std::cout << "Destructor" << std::endl;

}

// Copy Constructor
Matrix::Matrix(const Matrix &other) : num_rows{other.num_rows}, num_cols{other.num_cols} {

	this->allocateData();
	for (std::size_t row = 0; row < num_rows; ++row) {
		*(this->data.at(row)) = *(other.data.at(row));
	}

	// this->info = new MatrixInfo();
	*(this->info) = *(other.info);

	std::cout << "Copy Constructor" << std::endl;

}


// // Move Constructor
// No need for This because no allocated data...
// Matrix::Matrix(Matrix &&other) : num_rows{other.num_rows}, num_cols{other.num_cols} {

// 	other.num_cols = other.num_rows = 0;

// 	this->data = other.data;
// 	this->info = other.info;

// 	other.data = nullptr;
// 	other.info = nullptr;

// 	std::cout << "Move Constructor" << std::endl;
// }



// void swap(Matrix &first, Matrix &second) {
// 	using std::swap;

// 	swap(first.num_rows, second.num_rows);
// 	swap(first.num_cols, second.num_cols);
// 	swap(first.data, second.data);
// 	swap(first.info, second.info);

// }

// // Copy Assignment
// Matrix &Matrix::operator=(Matrix other) {

// 	swap(*this, other);

// 	std::cout << "Copy Assignment" << std::endl;

// 	return *this;

// }


/****************************************************************************\
|****************************************************************************|
\****************************************************************************/





/****************************************************************************\
|*************************************CSV************************************|
\****************************************************************************/


// Note, CSV functions assume no newline at end, may pose issue for unix file endings.


static size_t csvRowCount(const char *file_name) {
	FILE *fptr = fopen(file_name, "r");
	if (fptr == nullptr) throw FileNotFound();

	char c;
	std::size_t count = 1; // Assumes #rows >= 1
	while ( ((c = fgetc (fptr))!= EOF) ) {
		if (c == '\n') count++;
	}

	if ( ferror (fptr) ) throw FileReadError();
	fclose(fptr);

	return count;
}


static size_t csvColumnCount(const char *file_name) {
	FILE *fptr = fopen(file_name, "r");
	if (fptr == nullptr) throw FileNotFound();

	char c;
	std::size_t count = 1; // Assumes at least one column (non empty file). 
	while ( (c = fgetc (fptr)) != EOF ) {
		if (c == ',') count++;
		else if (c == '\n') break;
	}

	if ( ferror (fptr) ) throw FileReadError();
	fclose(fptr);

	return count;
}



void Matrix::readCSV(const char *file_name) {

	this->num_rows = csvRowCount(file_name);
	this->num_cols = csvColumnCount(file_name);
	this->allocateData();


	FILE *fp = fopen(file_name, "r");
	if (fp == nullptr) throw FileNotFound();

	std::size_t buff_size = (this->num_cols * sizeof(double) + 1) * TYPE_SIZE;
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		char *row_data = new char[buff_size];
		if (not fgets(row_data, buff_size, fp)) throw InvalidRead();

		char *token;
		token = std::strtok(row_data, SEP);
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			this->data[row]->set(col, ATOT(token));
			token = std::strtok(nullptr, SEP);
		}
		delete[] row_data;
	}

	fclose(fp);
}

/****************************************************************************\
|****************************************************************************|
\****************************************************************************/




/****************************************************************************\
|*********************************OPERATORS**********************************|
\****************************************************************************/
      

// No need to compare this->info, although may be faster to do that check.
bool Matrix::operator==(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) {
		return false;
	}
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			if (this->data[row]->get(col) != rhs.data[row]->get(col)) {
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator!=(const Matrix &rhs) const {
	return not (*this == rhs);
}



Matrix Matrix::operator*(const Matrix &rhs) const {

	if (this->num_cols != rhs.num_rows) throw InvalidDimensions();

	Matrix product = Matrix(this->num_rows, rhs.num_cols);
	for (std::size_t row = 0; row < product.num_rows; ++row) {
		for (std::size_t col = 0; col < product.num_cols; ++col) {
			double sum = 0;
			for (std::size_t k = 0; k < this->num_cols; ++k) {
				sum += this->data[row]->get(k) * rhs.data[k]->get(col);
			}
			product.data[row]->set(col, sum);
		}
	}

	return product;
}


Matrix Matrix::operator*(double scalar) const {
	
	Matrix product(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		*(product.data[row]) *= scalar;
	}
	return product;
}



Matrix Matrix::operator+(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) throw InvalidDimensions();
	
	Matrix sum(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		sum[row] += *(rhs.data[row]);	
	}
	return sum;
}


Matrix Matrix::operator+(double scalar) const {
	
	Matrix sum(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		sum[row] += scalar;	
	}
	return sum;
}




Matrix Matrix::Matrix::operator-(const Matrix &rhs) const {
	if (this->num_rows != rhs.num_rows or this->num_cols != rhs.num_cols) throw InvalidDimensions();
	
	Matrix difference(*this);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		difference[row] -= *(rhs.data[row]);
	}
	return difference;
}



Matrix Matrix::operator-(double scalar) const {
	return Matrix(*this + (-scalar));
}

VectorX &Matrix::operator[](std::size_t index) {
	return *(this->data[index]);
}


/****************************************************************************\
|*********************************** MISC ***********************************|
\****************************************************************************/

// Need to define "unpermute as well".
// Though if eventually multithreading gets involved, may need to 
// change function to return a new permuted matrix...
// fml
void Matrix::permute(VectorX permutation) {
	if (permutation.size() != this->getRows()) throw InvalidDimensions();

	std::vector<std::shared_ptr<VectorX>> swapper(this->getRows());
	for (std::size_t i = 0; i < this->getRows(); ++i) swapper[i] = this->data[i];
	for (std::size_t i = 0; i < this->getRows(); ++i) this->data[i] = swapper.at(permutation.get(i));

}



// Update all flags and update validState to true
void Matrix::revalidateState() {
	this->setIsUpper();
	this->setIsLower();
	this->setIsIdentity();
	this->setIsZero();
	this->info->validState = true;
}



// isUpperTriangular and isLowerTriangular duplicates code. oops.
bool Matrix::isUpperTriangular() {

	if (not this->info->validState) this->revalidateState();
	return this->info->isUpper;

}

// raw_* are only called when validState is false

// Updates isUpper flag
void Matrix::setIsUpper() {

	this->info->isUpper = false;
	std::size_t col = 0;
	for (std::size_t row = 0; row > col; ++row) {
		for (col = 0; col <= row; ++col) {
			if (this->data[row]->get(col) != 0) return;
		}
	}
	this->info->isUpper = true;
}

// isUpperTriangular and isLowerTriangular duplicates code. oops.
bool Matrix::isLowerTriangular() {

	if (not this->info->validState) this->revalidateState();
	return this->info->isLower;

}

// Updates isLower flag
void Matrix::setIsLower() {

	this->info->isLower = false;
	std::size_t col = 0;
	for (std::size_t row = 0; row > col; ++row) {
		for (col = row; col < this->num_cols; ++col) {
			if (this->data[row]->get(col) != 0) return;
		}
	}
	this->info->isLower = true;
}


bool Matrix::isZero() {
	if (not this->info->validState) this->revalidateState();
	return this->info->isZero;
}

void Matrix::setIsZero() {

	this->info->isZero = false;
	for (std::size_t row = 0; row < this-> num_rows; ++row) {
		for (std::size_t col = 0; col < this-> num_cols; ++col) {
			if (this->data[row]->get(col) != 0) return;
		}
	}
	this->info->isZero = true;

}

bool Matrix::isIdentity() {
	if (not this->info->validState) this->revalidateState();
	return this->info->isIdentity;
}


void Matrix::setIsIdentity() {
	this->info->isIdentity = false;
	if (num_cols != num_rows) return;

	for (std::size_t row = 0; row < this-> num_rows; ++row) {
		for (std::size_t col = 0; col < this-> num_cols; ++col) {
			if (row == col) {
				if (this->data[row]->get(col) != 1) return;
			} else if (this->data[row]->get(col) != 0) return;
		}
	}
	this->info->isIdentity = true;	

}



/****************************************************************************\
|***********************************TESTING**********************************|
\****************************************************************************/


void Matrix::print() const {
	std::cout.precision(2);
	for (std::size_t row = 0; row < this->num_rows; ++row) {
		for (std::size_t col = 0; col < this->num_cols; ++col) {
			this->data[row]->print();
		}
		// std::cout << std::endl;
	}

}




/****************************************************************************\
|**********************************ROUGHSTUFF********************************|
\****************************************************************************/



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


// // Move Assignment
// Matrix &Matrix::operator=(Matrix &&other) {
// 	swap(*this, other);
// 	std::cout << "Move Assignment" << std::endl;
// 	return *this;

// }



// double Matrix::dot(const Matrix &lhs, const Matrix &rhs, std::size_t row, std::size_t col) const {
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
// 			double sum = 0;
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
// 			double acc00 = 0, acc01 = 0, acc10 = 0, acc11 = 0;
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
// 			double sum = 0;
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
// 		double cell = 0;
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
