#ifndef _DECOMPOSITION_H_
#define _DECOMPOSITION_H_

#include <iostream>
#include "matrix.h"
#include <vector>
#include <cmath>

class SomeError {};


typedef std::vector<float> Vector;



Vector Pivot(Matrix &a);

 



// Lij = 1/Ujj(Aij - Sum[k=1,j-1](UkjLik))
void LUFactorize(Matrix &M, Matrix &L, Matrix &U);



// Solves Ax = b and returns x
Vector BackSolve(Matrix &A, Vector &b, Matrix &L, Matrix &U);
#endif
