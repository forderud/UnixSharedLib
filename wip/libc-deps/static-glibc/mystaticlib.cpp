#include <cassert>
#include <cstdint>
#include <fstream>
#include <stdio.h>
#include <string>
#include "mystaticlib.hpp"


int compute_sum_static (int a, int b) {
    printf("mystaticlib built against glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    printf("mystaticlib built against libstdc++ %d\n", _GLIBCXX_RELEASE);

    {
        printf("Testing file I/O...\n");
        std::ifstream file("document.txt");
        std::string line;
        while (std::getline(file, line))
            printf("file content: %s\n", line.c_str());
        printf("[success]\n\n");
    }

    printf("Computing the sum of %i and %i ...\n", a, b);
    int sum = a + b;
    printf("The sum is %i\n", sum);
    return sum;
}
