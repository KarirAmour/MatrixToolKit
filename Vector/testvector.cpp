#include "testvector.h"
#include "vector.h"
#include <vector>
#include <cassert>
#include <iostream>

double abs(double a) {
    return a > 0 ? a : -a;
}

bool equal(double a, double b) {
    double epsilon = 0.001;
    return abs(a - b) < epsilon;
}

bool TestVector::testBigFive() { // No need since automatically tested?
    std::cout << "testBigFive() ";

    // Default Constructor

    Vector *testZeroLength = new Vector(0);
    assert(testZeroLength->size() == 0);
    assert(testZeroLength->sum() == 0);
    assert(testZeroLength->squaredNorm() == 0);
    assert(testZeroLength->isBasisVector() == false);
    assert(testZeroLength->isZeroVector() == true);

    Vector *testOneLength = new Vector(1);
    assert(testOneLength->size() == 1);
    assert(testOneLength->sum() == 0);


    // Constructor with initializer


    // Construct from std::vector


    // Construct from Vector


    // GDB for memory leaks


    std::cout << "PASSED!" << std::endl;

    return false;
}



bool TestVector::testOperators() {


    // std::cout << "small1: " << std::endl;    
    Vector small1(5, 3);
    assert(small1.size() == 5);
    // small1.print();

    // std::cout << "small2: " << std::endl;    
    Vector small2(5, 3.0000);
    assert(small2.size() == 5);
    // small2.print();

    Vector small3(small2);
    std::cout << "small3 norm: "  << small3.squaredNorm() << std::endl;    
    assert(small3.size() == 5);    
    // small3.print();

    assert(small2.isZeroVector() == small1.isZeroVector());
    assert(small3.isZeroVector() == small2.isZeroVector());
    assert(small1.isBasisVector() == small2.isBasisVector());
    assert(small3.isBasisVector() == small2.isBasisVector());

    assert(equal(small2.sum(), small1.sum()));
    assert(equal(small2.sum(), small3.sum()));
    std::cout << small2.squaredNorm() << " " << small3.squaredNorm() << std::endl;        
    assert(equal(small2.squaredNorm(), small1.squaredNorm()));
    assert(equal(small2.squaredNorm(), small3.squaredNorm()));


    assert(small2 == small1);
    assert(! (small2 != small1));
    assert(small2 == small3);
    assert(! (small2 != small3));
    small3 = small2;
    assert(! (small1 != small3));
    assert(! (small3 != small2));
    assert(small1 == small3);
    assert(small3 == small2);
    small3.set(0, 3);
    small3.set(4, 3);
    small3.set(1, 3);
    assert(small3 == small2);
    small3 *= 1;
    assert(small3 == small2);
    small3 += 0;
    assert(small2 == small3);
    assert(small3 == small1);
    small3 += 3;
    small2 *= 2;
    assert(small2 == small3); 
    assert(! (small3 != small2));
    assert(small1 != small2);
    assert(small3 != small1);
    small2.print();
    small3.print();
    std::cout << (small3 * small3) << " " << small3.squaredNorm() << std::endl;
    assert((small3 * small3) == small3.squaredNorm());
    assert((small3 * small3) == small3.squaredNorm());
    assert((small3 * small2) == small2.squaredNorm());

    // Assignment operator



    // Equality operator



    // Not equal Operator



    std::cout << "testOperators() ";
    std::cout << "PASSED!" << std::endl;

    return false;
}

bool TestVector::testMethods() {


    return false;
}

bool TestVector::testGetSet() {
    
    return false;

}


void TestVector::Test() {

    testBigFive();

    testOperators();

    // testMethods();

    // testGetSet();
}
