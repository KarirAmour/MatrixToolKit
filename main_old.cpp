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

void test_intSize() {
	if (intSize(0) != 1) std::cout << "0" << std::endl;
	if (intSize(1) != 1) std::cout << "1" << std::endl;
	if (intSize(12) != 2) std::cout << "2" << std::endl;
	if (intSize(1299) != 4) std::cout << "4" << std::endl;
	if (intSize(2922828) != 7) std::cout << "8" << std::endl;
}

void changeName(char *str, int loc, int suffix) {
	int end = intSize(suffix);
	for (int i = end - 1; i >= 0; --i) {
		str[loc + i] = '0' + (suffix % 10);
		suffix /= 10;
	}


}

bool strequal(char *a, char *b) {
	while (true) {
		if (*a != *b) return false;
		if (*a == '\0') return true;
		++a;
		++b;
	}
}

void test_changeName() {

	char a[4] = "abc";
	char b[6] = "abcde";
	char c[8] = "abcdefg";
	char d[12] = "abcdefghijk";

	char a_test[4] = "ab0";
	char b_test[6] = "ab123";
	char c_test[8] = "a123456";
	char d_test[12] = "abc12345678";	

	changeName(a, 2, 0);
	changeName(b, 2, 123);
	changeName(c, 1, 123456);
	changeName(d, 3, 12345678);

	if (not strequal(a, a_test)) std::cout << a << std::endl;
	if (not strequal(b, b_test)) std::cout << b << std::endl;
	if (not strequal(c, c_test)) std::cout << c << std::endl;
	if (not strequal(d, d_test)) std::cout << d << std::endl;


}


int main() {

	test_intSize();
	test_changeName();

	float t1 = 0, t2 = 0;
	int num_iter = 90;
	int size = 500;
	int start = 110;
	char name[28] = "testing/matrix_mult_test___";
	for (int suff = start; suff < (start + num_iter); ++suff) {

		changeName(name, 24, suff);
		Matrix a(name, size,size);

		// clock_t clk2 = clock();
		// Matrix n2 = a.naiveLoopMultiply(a);
		// clk2 = clock() - clk2;
		// t2 += ((float)clk2)/CLOCKS_PER_SEC;

		clock_t clk1 = clock();
		Matrix n1 = a.naiveMultiply(a);
		clk1 = clock() - clk1;
		t1 += ((float)clk1)/CLOCKS_PER_SEC;



		std::cout << suff << " >>> " << "t1: " << t1 << " ||| " << "t2: " << t2 << std::endl;


	}

	// for 90 500x500 matrices  16.3135 8.64903
	// 							14.181 7.64458
	//							14.2372 7.61113


	// for 10 1000x1000 matric  20.7007 12.7552
	//							21.3619 12.9138
	//							21.4316 12.92


	// naiveLoop swapped loops: 14.5133
	//							15.477
	//							14.6539

	// if (c == d) std::cout << "c == d" << std::endl;
	// t = clock() - t;
	// std::cout << "All " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	// char l[10] = "multTestXXXX";
	// int b = 77;
	// char a = b;
	// std::cout << b << " " << a << std::endl;
	// std::cout << l << std::endl;
	// // l[1] = char(7 + '0');
	// changeName(l, 1, 7);
	// std::cout << l << std::endl;



	return 0;
}

