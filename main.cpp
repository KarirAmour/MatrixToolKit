#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>


#include "matrix.h"

// using namespace std;

int main() {

	Matrix m("1000_1000", 1000, 1000);
	// m.print();

	Matrix n(m);

	if (m == n) {
		std::cout << "m == n" << std::endl;
	}
	return 0;
}