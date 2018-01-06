#include "vector_test.h"
#include "vector.h"
#include <vector>
#include <cassert>

bool TestVector::testBigFive() { // No need since automatically tested?

    // Default Constructor

    Vector *testZeroLength = new Vector(0);
    assert(testZeroLength->size() == 0);
    assert(testZeroLength->Sum() == 0);
    assert(testZeroLength->squaredNorm() == 0);
    assert(testZeroLength->isBasisVector() == false);
    assert(testZeroLength->isZeroVector() == false);

    Vector *testOneLength = new Vector(1);
    assert(testOneLength->size() == 1);
    assert(testOneLength->Sum() == 0);


    // Constructor with initializer


    // Construct from std::vector


    // Construct from Vector


    // GDB for memory leaks

    return false;
}

bool TestVector::testOperators() {


    // Assignment operator



    // Equality operator



    // Not equal Operator

    return false;
}

bool TestVector::testMethods() {


    return false;
}

bool TestVector::testGetSet() {
    
    return false;

}


TestVector::TestVector() {

    testBigFive();

    testOperators();

    testMethods();

    testGetSet();
}