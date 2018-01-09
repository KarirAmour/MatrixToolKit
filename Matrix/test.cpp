#include <iostream>
#include "matrix.h"
#include "decomposition.h"
// #include "../Vector/vector.h"


int main() {
	Matrix A("ludec2.txt");
	A.print();

	// Matrix *L, *U;
	Matrix L(A.getRows(), A.getCols());
	Matrix U(A.getRows(), A.getCols());
	std::cout << "L:"<< std::endl;
	// LUFactorize(A, L, U);

	// std::vector<float> b{3, 13, 4};
	// // LUFactorize(A, L, U);
	// std::vector<float> y;
	// y = BackSolve(A, b, L, U);

	// std::cout << "L:"<< std::endl;
	// L.print();
	// std::cout << "U:"<< std::endl;
	// U.print();


	// Pivot(a);



	Matrix b("ludec2.txt");
	b.print();

	// Vector perm = Pivot(b);

	// b.permute(perm);
	b.print();



	return 0;
}