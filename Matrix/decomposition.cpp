#include "decomposition.h"
#include "info.h"
// #include <vector>
#include "../Vector/vector.h"

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


Vector<int> Pivot(Matrix &m) {
	if (m.getRows() != m.getCols()) throw InvalidDimensions(); // Need SQ matrix

	// permutation = {col1, col2, col3, ... coln} where the value of coli corresponds
	// to the column number that is set to 1 in the ith row.
	Vector<int> permutation = Vector<int>(m.getRows());
	for (int i = 0; i < permutation.size(); ++i) permutation[i] = i;

	// Set the ith diagonal value to the largest element in column i.
	// Avoid infinite swapping by starting at row = col at each iteration.
	for (std::size_t col = 0; col < m.getCols(); ++col) {
		int max_row = col;
		for (std::size_t row = col; row < m.getRows(); ++row) {
			if (fabs(m[row][col]) > fabs(m[max_row][col])) {
				max_row = row;
			}
		}
		if (max_row != col) { // Swap rows if it is not currently largest
			int swp = permutation[col];
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
	// std::cout << "L SIZE: " << L.getRows() << ", " << L.getRows() << std::endl;
	// std::cout << "U SIZE: " << U.getRows() << ", " << U.getRows() << std::endl;

	Vector<int> permutation = Pivot(A); // Pivot based on largest elements
	A.permute(permutation);	// Permute A based on perm
	std::cout << "Permuted A." << std::endl;
	A.print();

	for (std::size_t row = 0; row < A.getRows(); ++row) {
		// std::cout << "ROW: " << row << std::endl;
		for (std::size_t col = 0; col < A.getCols(); ++col) {
			// std::cout << "COL: " << col << std::endl;

			float sum;
			if (col <= row) {
				sum = 0;
				for (std::size_t k = 0; k < col; ++k) {
					sum += L[col][k] * U[k][row];
					// std::cout << "K1: " << k << std::endl;

				}
				// float temp = ;
				U[col][row] = A[col][row] - sum;
			}
			if (col >= row) {
				sum = 0;
				for (std::size_t k = 0; k < row; ++k) {
					// std::cout << "K2: " << k << ", " << row << ", " << col << std::endl;

					sum += L[col][k] * U[k][row];
				}
				L[col][row] = (A[col][row] - sum) / U[row][row];
			}
		}

	}
	std::cout << "L:" << std::endl;
	L.print();
	std::cout << "U:" << std::endl;
	U.print();
	std::cout << "P:" << std::endl;
	printVector(permutation);

}

Vector BackSolve(Matrix &A, Vector &b, Matrix &L, Matrix &U) {
	if (A.getRows() != A.getCols() or A.getRows() != b.size()) 
		throw InvalidDimensions();
	
	int dim = b.size();

	// LU Decompose A into Lower L and Upper U
	// Matrix L(dim, dim);
	// Matrix U(dim, dim);
	LUFactorize(A, L, U);
	L.print();
	U.print();
	std::cout << "Factorized." << std::endl;

	 // Solve Ly = b for y
	Vector y(b.size());
	std::cout << "y.size(): " << y.size() << std::endl;

	if (L[0][0] != 1) throw SomeError();
	y[0] = b[0];
	std::cout << "y[0] set." << std::endl;
	std::cout << "Dim: " << dim << std::endl;

	for (int i = 0; i < dim; ++i) {
		std::cout << "ROW: " << i << std::endl;

		float sum = 0;
		for (int j = 0; j < (i - 1); ++j) {
			std::cout << "COL: " << j << std::endl;
			sum += L[i][j] * y[j];
		}
		y[i] = sum;
	}
	printVector(y);

	//  // Solve Ux = y for x
	// Vector x = Vector(dim);
	// if (U[dim - 1][dim - 1] != 1) throw SomeError();
	// y[dim - 1] = b[dim - 1];

	// for (std::size_t i = dim - 1; i != 0; --i) {
	// 	float sum = 0;
	// 	for (std::size_t j = 0; j < (i - 1); ++i) {
	// 		sum += U[i][j] * x[j];
	// 	}
	// }

	return y;
}