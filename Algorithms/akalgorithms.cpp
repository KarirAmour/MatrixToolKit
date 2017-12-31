#ifndef _KAHANSUM_H_
#define _KAHANSUM_H_

#include <iostream>
#include <vector>
#include <string>



template< typename Container, typename Type = typename Container::value_type >
Type KahanSum(Container &list) {
	clock_t t = clock();

	Type sum = 0, error = 0, diff, temp;
	for (std::size_t i = 0; i < list.size(); ++i) {
		diff = list[i] - error;
		temp = sum + diff;
		error = (temp - sum) - diff;
		sum = temp;
	}
	t = clock() - t;
	std::cout << "KahanSum Time: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
	return sum;
}

template< typename Container, typename Type = typename Container::value_type >
Type neumaierSum(Container &list) {
	clock_t t = clock();

	Type sum = list[0];
	Type error = 0;
	Type temp;
	for (int i = 1; i < list.length; ++i) {
		temp = sum + list[i];
		if ( myabs(sum) >= myabs(list[i]) ) error += (sum - temp) + list[i];
		else error += (list[i] - temp) + sum;
		sum = temp;
	}

	t = clock() - t;
	std::cout << "neuma: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	return sum + error;
}

template< typename Container, typename Type = typename Container::value_type >
Type naiveSum(Container &list) {
	clock_t t = clock();

	Type sum = 0;
	for (int i = 0; i < list.length; ++i) {
		sum += list.data[i];
	}

	t = clock() - t;
	std::cout << "naive: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
	
	return sum;
}




unsigned int ak_logarithm(unsigned int base, unsigned int value) {
	int n = 0;
	while (value /= base) ++n;
	return n;
}

unsigned int ak_exponentiate(unsigned int base, unsigned int exponent) {
	int n = 1;
	while (exponent--) n *= base;
	return n;
}

void toBase(unsigned int base, unsigned int value) {
	std::cout << value << " 	in base " << base << " is: ";

	unsigned int exp = ak_logarithm(base, value);
	// std::cout << "exp: " << exp << std::endl;

	std::string inBase(exp + 1, ' ');

	unsigned int placeholder = ak_exponentiate(base, exp);
	// std::cout << "placeholder: " << placeholder << std::endl;

	unsigned int iter = 0;
	while (iter <= exp) {
		// std::cout  << value << " / " << placeholder << ": " << value / placeholder << std::endl;
		inBase[iter++] = '0' + (value / placeholder);
		value %= placeholder;
		placeholder /= base;
	}
	std::cout << inBase << std::endl;

}




int main() {

	// // Doesn't work due to compiler being too smart :)
	// std::vector<double> table = { 
	// 	1.0001, -2000000.1234, 0.9999, 2000000.1234, 4.0000001, -2, -4.0000001 };
	// const int size = 63000000;
	// std::vector<double> toSum;
	// toSum.reserve(size);	
	// for (int i = 0; i < size; ++i) {
	// 	toSum[i] = table[i % 7];


	// }
	// double b = KahanSum(toSum);
	// std::cout << b << std::endl;


	std::cout << ak_logarithm(2, 64) << std::endl;
	std::cout << ak_logarithm(4, 64) << std::endl;
	std::cout << ak_logarithm(5, 64) << std::endl;
	std::cout << ak_logarithm(7, 64) << std::endl;
	std::cout << std::endl;
	std::cout << ak_exponentiate(2, 6) << std::endl;
	std::cout << ak_exponentiate(4, 3) << std::endl;
	std::cout << ak_exponentiate(5, 2) << std::endl;
	std::cout << ak_exponentiate(13, 2) << std::endl;
	std::cout << std::endl;
	toBase(2, 1);
	toBase(2, 8);
	toBase(2, 15);
	toBase(10, 123456);
	// std::cout << ak_exponentiate(10, 123456) << std::endl;
	return 0;

}



#endif
