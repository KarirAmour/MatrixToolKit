#include <iostream>
#include "matrix.h"
#include "decomposition.h"
// #include "../Vector/vector.h"


int main() {
	Matrix A("ludec.txt");
	A.print();

	// Matrix *L, *U;
	Matrix L(A.getRows(), A.getCols());
	Matrix U(A.getRows(), A.getCols());
	LUFactorize(A, L, U);

	L.print();
	U.print();

	// Pivot(a);



	// Matrix b("ludec2.txt");
	// b.print();

	// Vector perm = Pivot(b);

	// b.permute(perm);
	// b.print();



	return 0;
}