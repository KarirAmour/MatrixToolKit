#include "decomposition.h"
#include "info.h"
#include <vector>



// First LU Decompose
// A = LU
// Uij = Aij - Sum[k=1,i-1](UkjLik)
// Lij = 1/Ujj(Aij - Sum[k=1,j-1](UkjLik))


void printVector(Vector &v) {
	std::cout << "PRINT VECTOR" << std::endl;
	for (std::size_t i = 0; i < v.size(); ++i) {
		std::cout << "(" << i << ", " << v[i] << ") ";
	}
	std::cout << std::endl;
}


Vector Pivot(Matrix &a) {
	if (a.getRows() != a.getCols()) throw InvalidDimensions(); // Need SQ matrix

	// permutation = {col1, col2, col3, ... coln} where the value of coli corresponds
	// to the column number that is set to 1 in the ith row.
	Vector permutation(a.getRows());
	for (std::size_t i = 0; i < permutation.size(); ++i) permutation[i] = i;

	// Set the ith diagonal value to the largest element in column i.
	// Avoid infinite swapping by starting at row = col at each iteration.
	for (std::size_t col = 0; col < a.getCols(); ++col) {
		int max_row = col;
		for (std::size_t row = col; row < a.getRows(); ++row) {
			if (fabs(a[row][col]) > fabs(a[max_row][col])) {
				max_row = row;
			}
		}
		if (max_row != col) { // Swap rows if it is not currently largest
			float swp = permutation[col];
			permutation[col] = permutation[max_row];
			permutation[max_row] = swp;
		}
	}

	printVector(permutation);

	return permutation;

}



// First LU Decompose
// A = LU
// Urc = Arc - Sum[k=1,r-1](Ukc*Lrk)

// Matrix &UpperFactorize(const Matrix &A, Matrix &L, Matrix &U) {
// 	if (A.getRows() != A.getCols()) throw InvalidDimensions();

// 	Matrix upper(A);
// 	for (std::size_t row = 0; row < upper.getRows(); ++row) {
// 		for (std::size_t col = 0; col < upper.getCols(); ++col) {
// 			float sum = 0;
// 			for (std::size_t k = 0; k < (row - 1); ++row) {
// 				sum += upper[k][c] 
// 			}

// 		}
// 	}

// }

void LUFactorize(Matrix &A, Matrix &L, Matrix &U) {
	if (A.getRows() != A.getCols()) throw InvalidDimensions();

	// if (L or U) throw "L and U should not point to memory.";

	// L = new Matrix(A.getRows(), A.getCols());
	// U = new Matrix(A.getRows(), A.getCols());

	// Set L to Identity and U to Zero
	for (std::size_t row = 0; row < A.getRows(); ++row) {
		for (std::size_t col = 0; col < A.getCols(); ++col) {
			L[row][col] = (row == col); // Identity
			U[row][col] = 0;
		}
	}
	std::cout << "L SIZE: " << L.getRows() << ", " << L.getRows() << std::endl;
	std::cout << "U SIZE: " << U.getRows() << ", " << U.getRows() << std::endl;

	Vector permutation = Pivot(A); // Pivot based on largest elements
	A.permute(permutation);	// Permute A based on perm
	std::cout << "Permuted A." << std::endl;

	for (std::size_t row = 0; row < A.getRows(); ++row) {
		std::cout << "ROW: " << row << std::endl;

		for (std::size_t col = 0; col < A.getCols(); ++col) {
			std::cout << "COL: " << col << std::endl;

			float sum;
			if (col <= row) {
				sum = 0;
				for (std::size_t k = 0; k < col; ++k) {
					sum += L[col][k] * U[k][row];
					std::cout << "K1: " << k << std::endl;

				}
				// float temp = ;
				U[col][row] = A[col][row] - sum;
			}
			if (col >= row) {
				sum = 0;
				for (std::size_t k = 0; k < row; ++k) {
					std::cout << "K2: " << k << ", " << row << ", " << col << std::endl;

					sum += L[col][k] * U[k][row];
				}
				L[col][row] = (A[col][row] - sum) / U[row][row];
			}
		}

	}

}
