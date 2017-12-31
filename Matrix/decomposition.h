#ifndef _DECOMPOSITION_H_
#define _DECOMPOSITION_H_

#include <iostream>
#include "matrix.h"
#include <vector>
#include <cmath>



typedef std::vector<float> Vector;



Vector Pivot(Matrix &a);

 



// Lij = 1/Ujj(Aij - Sum[k=1,j-1](UkjLik))
void LUFactorize(Matrix &M, Matrix &L, Matrix &U);



// Solves Ax = b and returns x
// Vector backSolve(Matrix &m, Vector &b);
// Vector backSolve(Matrix &m, Vector &b) {
// 	if (m.getRows() != m.getCols() or m.getRows() != b.size()) 
// 		throw InvalidDimensions();

// 	Vector a = Vector(b.size());
// 	return a;
// }

#endif
