#include "info.h"

std::size_t nextPowerOfTwo(std::size_t value) {
    if (value == 0) return 1;
    std::size_t next = 2;
    while (value >>= 1) next <<= 1;
    return next;
}