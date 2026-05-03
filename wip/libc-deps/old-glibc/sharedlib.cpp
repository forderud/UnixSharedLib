#include <cassert>
#include <cstdint>
#include <stdio.h>
#include <vector>
#include "sharedlib.hpp"


int compute_sum (int a, int b) {
    printf("sharedlib built against glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    printf("sharedlib built against libstdc++ %d\n", _GLIBCXX_RELEASE);

#if 1 // enable to trigger glibc errors
    {
        printf("Testing dynamic memory allocation...\n");
        std::vector<uint8_t> buffer(1024*1014, (uint8_t)0);
        assert(buffer[1024] == 0);
        printf("[success]\n\n");
    }
#endif

    int sum = a + b;
    printf("sharedlib returned %i\n", sum);
    assert(sum == 7);
    return sum;
}
