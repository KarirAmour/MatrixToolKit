#include <iostream>
#include <vector>
#include <cassert>

#include "vector.h"
#include "testvector.h"


// using namespace std;




int main() {

    TestVector test;
    test.Test();

    return 0;
}

/*
    cout << "Test 1." << endl;
    Vector v(10);
    assert(v.size() == 10);
    v.print();

    cout << "Test 2." << endl;
    Vector v1(300, 15.001);
    assert(v1.size() == 300);

    cout << "Test 3." << endl;    
    std::vector<double> vc1(300, 15.001);

    Vector vcc1(vc1);
    // assert(v1 == vcc1);
    // assert(not (v1 != vcc1));
    cout << "Test 4." << endl;
    Vector v2;
    for (std::size_t i = 0; i < 300; ++i) {
        v2.append(i); 
    }
    for (std::size_t i = 0; i < 300; ++i) {
        assert(v2.get(i) == i); 
    }

    Vector v3(3000, 0);
    std::cout << "set(1,1)" << std::endl;
    assert(v3.isZeroVector());
    v3.set(1, 1);
    assert(v3.isBasisVector());
    assert(v3.squaredNorm() == 1);
    assert(v3.sum() == 1);
    
    v3.set(1, 1);
    std::cout << "set(1,1) 2" << std::endl;
    assert(not v3.isZeroVector());
    assert(v3.isBasisVector());
    assert(v3.squaredNorm() == 1);
    assert(v3.sum() == 1);

    v3.set(2999, 2);
    std::cout << "set(2999,2)" << std::endl;
    assert(not v3.isZeroVector());
    assert(not v3.isBasisVector());
    assert(v3.squaredNorm() == 5);
    assert(v3.sum() == 3);

    v3.set(1, 0);
    std::cout << "set(1,0)" << std::endl;
    assert(not v3.isZeroVector());
    assert(v3.isBasisVector());
    assert(v3.squaredNorm() == 4);
    assert(v3.sum() == 2);

    v3.set(2999, 0);
    assert(v3.isZeroVector());
    assert(not v3.isBasisVector());
    assert(v3.squaredNorm() == 0);
    assert(v3.sum() == 0);
*/

