#include <cassert>
#include <cstdint>
#include <future>
#include <string>
#include <vector>
#include "ModernStuff.hpp"

class InternalRttiClass {
public:
    InternalRttiClass() {
        printf("ctor\n");
    }
    virtual ~InternalRttiClass() {
        printf("dtor\n");
    }
};

int compute_sum (int a, int b) {
    printf("ModernStuff built against glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);
    printf("ModernStuff built against libstdc++ %d\n", _GLIBCXX_RELEASE);
	printf("ModernStuff C++ ABI %u\n", __GXX_ABI_VERSION);

    {
        printf("Testing dynamic memory allocation...\n");
        std::vector<uint8_t> buffer(1024*1014, (uint8_t)0);
        assert(buffer[1024] == 0);
        printf("[success]\n");
    }
    {
        std::string str = "Hello from string!";
        printf("String content: %s\n", str.c_str());

        if (str.empty())
            throw std::runtime_error("string missing");
    }
    {
        printf("Testing std::async...\n");
        std::future<int> multiply = std::async(std::launch::async, [](int a, int b){
            return a * b;
        }, 3, 4);
        auto val = multiply.get();
        assert(val == 12);
        printf("[success]\n");
    }

    // test RTTI
    printf("Typeid: %s\n", typeid(InternalRttiClass).name());

    return 12;
}
