#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include "mysharedlib.hpp"
#include "mystaticlib.hpp"


int compute_sum (int a, int b) {
    {
        printf("Testing dynamic memory allocation...\n");
        std::vector<uint8_t> buffer(1024*1014, (uint8_t)0);
        assert(buffer[1024] == 0);
        printf("[success]\n\n");
    }

    printf("Forwarding computation request to mystaticlib...\n");
    int sum = compute_sum_static(a, b);
    printf("mystaticlib returned %i\n", sum);
    return sum;
}

void test_internal_exception () {
    printf("Testing internal exception throw & catch..\n");
    
    try {
        throw_standard_exception("Some error");
    } catch (const std::exception& e) {
        printf("Expected exception: %s\n", e.what());
    }

    try {
        throw_custom_exception("Some error");
    } catch (const dcm::bad_type& e) {
        printf("Expected exception: %s\n", e.what());
    }

    printf("[done]\n");
}
