#include <cassert>
#include <cstdint>
#include <stdio.h>
#include "mystaticlib.hpp"


int compute_sum_static (int a, int b) {
    printf("Computing the sum of %i and %i ...\n", a, b);
    int sum = a + b;
    printf("The sum is %i\n", sum);
    return sum;
}

void throw_standard_exception (const char* message) {
    throw std::runtime_error(message);
}

void throw_custom_exception (const char* message) {
    throw dcm::bad_type("bad_type problem");
}
