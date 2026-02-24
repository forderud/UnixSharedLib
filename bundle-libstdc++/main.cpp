#include <stdio.h>
#include <assert.h>
#include <cstdint>
#include <fstream>
#include "ModernStuff.hpp"

int main () {
    printf("executable built against glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    printf("executable built against libstdc++ %d\n", _GLIBCXX_RELEASE);
	printf("executable C++ ABI %u\n", __GXX_ABI_VERSION);

    int sum = compute_sum(3, 4);
    printf("Return-value %i\n", sum);

    {
        std::ifstream file("README.md");
        if (!file.is_open()) {
            printf("ERROR: Unable to open file!\n");
        }
    }
    
    return 0;
}
