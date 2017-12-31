#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

double randomnumgen(double low, double high) {
  return fmod(rand(), high - low) + low;

}


struct List {
	double *data;
	int length;
	List(int len, bool random = false, int min = 0, int max = 1) : length{len} {
		data = new double[length];

		if (random) {
			std::uniform_real_distribution<double> unif(min, max);
			std::default_random_engine re;
			for (int i = 0; i < length; ++i) data[i] = unif(re);
		}
	}

	List(double val, int length) : length{length} {
		std::cout << val << std::endl;
		data = new double[length];
		for (int i = 0; i < length; ++i) {
			data[i] = val;
		}
	}

	void print() {
		std::cout.precision(7);
		for (int i = 0; i < length; ++i) {
			std::cout << " " << data[i];
			if ((i+1) % 3 == 0) std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	~List() { delete[] data; }
};


// double pairwiseSum(List &list, int index, int length) {
// 	if (index <= 2) {
// 		return 
// 	}
// }


double naiveSum(List &list) {
	clock_t t = clock();

	double sum = 0;
	for (int i = 0; i < list.length; ++i) {
		sum += list.data[i];
	}

	t = clock() - t;
	std::cout << "naive: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
	
	return sum;
}


double kahanSum(List &list) {
	clock_t t = clock();

	double sum = 0, error = 0;
	double diff, temp;
	for (int i = 0; i < list.length; ++i) {
		diff = list.data[i] - error;
		temp = sum + diff;
		error = (temp - sum) - diff;
		sum = temp;
	}
	t = clock() - t;
	std::cout << "kahan: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;
	return sum;
}

double myabs(double val) {
	if (val < 0) return -val;
	return val;
}

double neumaierSum(List &list) {
	clock_t t = clock();

	double sum = list.data[0];
	double error = 0;
	double temp;
	for (int i = 1; i < list.length; ++i) {
		temp = sum + list.data[i];
		if ( myabs(sum) >= myabs(list.data[i]) ) error += (sum - temp) + list.data[i];
		else error += (list.data[i] - temp) + sum;
		sum = temp;
	}

	t = clock() - t;
	std::cout << "neuma: " << t << ", " << ((float)t)/CLOCKS_PER_SEC << std::endl;

	return sum + error;
}





int main() {


	/****************************************
	**       |    Time    |   Accuracy   | **
	** ----------------------------------- **
	** Naive |  0.356299  | 99.9999998%  | **
	** Kahan |  1.159426  |     100%     | **
	** Neuma |  1.106952  |     100%     | **
	** ------|------------|--------------| **
	****************************************/

	// List a = List(10, true, 0, 20);
	// a.print();

	// Don't increase vector size any more. Will crash computer.
	// Should use a generator expression...
	List b = List(.00000001234567890, 100000000); 
	double naive = naiveSum(b);
	double kahan = kahanSum(b);
	double neuma = neumaierSum(b);
	std::cout.precision(10);
	std::cout << "naive: " << naive << std::endl;
	std::cout << "kahan: " << kahan << std::endl;
	std::cout << "neuma: " << neuma << std::endl;
/*
	List b = List(100000000, true, 0, 2); 
	double naive = naiveSum(b);
	double kahan = kahanSum(b);
	double neuma = neumaierSum(b);
	std::cout.precision(10);
	std::cout << "naive: " << naive << std::endl;
	std::cout << "kahan: " << kahan << std::endl;
	std::cout << "neuma: " << neuma << std::endl;
*/

	return 0;
}





/*
Results:

Average(0.353434, 0.357482, 0.349773, 0.375227, 0.345578) = 0.3562988
Average(1.16171,  1.15836,  1.15702,  1.16223,  1.15781) = 1.159426
Average(1.0769 + 1.07749 + 1.12011 + 1.08494 + 1.17532) = 1.106952

*/