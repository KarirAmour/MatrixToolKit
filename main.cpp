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


	Matrix a("m1.txt", 3, 3);
	a.print();
	Matrix b = a - 5;
	b.print();
	Matrix c = a - b;
	c.print();
	if (a != a) std::cout << "a != a" << std::endl;
	if (a != b) std::cout << "a != b" << std::endl;
	// Matrix b = a * a;
	// Matrix c(b);
	// Matrix d = c;
	// d.print();
	// b.print();


	return 0;
}

