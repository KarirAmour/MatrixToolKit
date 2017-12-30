#include <iostream>
#include <string>
#include <ctime>


#include "matrix.h"

// using namespace std;
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


	return 0;
}

