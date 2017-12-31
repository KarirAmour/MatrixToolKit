#include <iostream>
#include <string>
#include <ctime>

#include <vector>
#include "matrix.h"
#include "vector.h"

using namespace std;
int intSize(int n) {
	// if (not n) return 0;

	int sz = 1;
	while (n /= 10) {
		++sz;
	}
	return sz;
}


void changeName(char *str, int loc, int suffix) {
	int end = intSize(suffix);
	for (int i = end - 1; i >= 0; --i) {
		str[loc + i] = '0' + (suffix % 10);
		suffix /= 10;
	}
}

Matrix copyAssignment(Matrix b) {
	return b * b;
}
bool strequal(char *a, char *b) {
	while (true) {
		if (*a != *b) return false;
		if (*a == '\0') return true;
		++a;
		++b;
	}
}





int main() {


	std::vector<float> a1 = {0.1, 1.11, 1.2, 213.02131};
	Vector b1(a1);
	cout << "(Capacity, Size) : (" << b1.getCapacity() << ", " << b1.size() << ") || Zero,Basis: " << b1.isZeroVector() << " " << b1.isBasisVector() << endl;
	b1.print();
	cout << endl;
	std::vector<float> a2 = {0.00000000000001, 0, 0, 0, 0};
	Vector b2(a2);
	cout << "(Capacity, Size) : (" << b2.getCapacity() << ", " << b2.size() << ") || Zero,Basis: " << b2.isZeroVector() << " " << b2.isBasisVector() << endl;
	b2.print();
	cout << endl;
	std::vector<float> a3 = {1};
	Vector b3(a3);
	cout << "(Capacity, Size) : (" << b3.getCapacity() << ", " << b3.size() << ") || Zero,Basis: " << b3.isZeroVector() << " " << b3.isBasisVector() << endl;
	b3.print();

	std::vector<float> a4 = {0};
	Vector b4(a4);
	cout << "(Capacity, Size) : (" << b4.getCapacity() << ", " << b4.size() << ") || Zero,Basis: " << b4.isZeroVector() << " " << b3.isBasisVector() << endl;
	b4.print();
	for (int i = 1; i < 24; ++i) b4.append(i);
	cout << "(Capacity, Size) : (" << b4.getCapacity() << ", " << b4.size() << ") || Zero,Basis: " << b4.isZeroVector() << " " << b3.isBasisVector() << endl;

	b4.print();



	// Vector::Test();
	// Vector::BenchMark();


	return 0;
}


/*
	// std::cout << "m1: " << csvColumnCount("m1.txt") << " " << csvRowCount("m1.txt") << std::endl;
	// std::cout << "m2: " << csvColumnCount("m2.txt") << " " << csvRowCount("m2.txt") << std::endl;
	// std::cout << "m3: " << csvColumnCount("m3.txt") << " " << csvRowCount("m3.txt") << std::endl;

	Matrix a("m1.txt");
	// a.info->isIdentity = true;
	// a.print();
	// Matrix b;
	// b = a;
	// if (b.info->isIdentity) std::cout << "b is identity." << std::endl;
	// // a.info->isIdentity = false;
	// if (b.info->isIdentity) std::cout << "b is identity." << std::endl;
	// Matrix c(a);
	// if (c.info->isIdentity) std::cout << "c is identity." << std::endl;
	a.print();
	Matrix b = a - 5;
	b.print();
	b = a;
	b.print();
	Matrix c = a - b;
	c.print();
	c = copyAssignment(b);

	// if (a != a) std::cout << "a != a" << std::endl;
	// if (a != b) std::cout << "a != b" << std::endl;
	// Matrix b = a * a;
	// Matrix c(b);
	// Matrix d = c;
	// d.print();
	// b.print();
*/