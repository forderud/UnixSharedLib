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
    {
        std::string libName = "ModernStuff";
        printf("%s built against glibc %d.%d\n", libName.c_str(), __GLIBC__, __GLIBC_MINOR__);
        printf("%s built against libstdc++ %d\n", libName.c_str(), _GLIBCXX_RELEASE);
        printf("%s C++ ABI %u\n", libName.c_str(), __GXX_ABI_VERSION);
    }
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
