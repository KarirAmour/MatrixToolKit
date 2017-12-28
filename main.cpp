#include <iostream>
#include <string>
#include <ctime>


#include "matrix.h"

// using namespace std;

int main() {

	Matrix m("1000_1000", 1000, 1000);
	// m.print();

	Matrix n(m);

	// if (m == n) {
	// 	std::cout << "m == n" << std::endl;
	// }
	clock_t t = clock();
	Matrix c = n * m;
	t = clock() - t;
	std::cout << "All " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	return 0;
}