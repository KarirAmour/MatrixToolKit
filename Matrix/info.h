#ifndef _INFO_H_
#define _INFO_H_

#include <cstdlib>

#define TYPE double
#define TYPE_SIZE 16
#define ATOT atof
#define SEP (",")


class FileNotFound {};
class FileReadError {};
class NoMemory {};
class InvalidRow {};
class InvalidCol {};
class InvalidRead {};
class InvalidDimensions {};
class CalculationError {};
class OutOfBoundsException {};
class DivisionByZeroError {};


std::size_t nextPowerOfTwo(std::size_t value);


#endif
