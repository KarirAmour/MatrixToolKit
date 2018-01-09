#include <iostream>
#include <vector>
#include <cassert>
#include <tgmath.h>

#include "vector.h"

using namespace std;

int next_pow_2(int x) {
    int power = 2;
    while (x >>= 1) power <<= 1;
    return power;
}

unsigned int nextPowerof2(std::size_t value) {
    if (value == 0) return 1;
    unsigned int next = 2;
    while (value >>= 1) next <<= 1;
    return next;
}
int main() {


    cout << endl <<endl;
    for (std::size_t i = 1; i < 513; i *= 2) {
        cout << "next power (" << i-1 << ") = " <<  next_pow_2(i-1) << endl;
        cout << "next power (" << i << ") = " << next_pow_2(i) << endl;
        cout << "next power (" << i+1 << ") = " << next_pow_2(i+1) << endl;
    };
    
    return 0;
}


