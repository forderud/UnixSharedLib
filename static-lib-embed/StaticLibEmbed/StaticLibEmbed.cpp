#include <cstdio>
#include <MyStaticLib.hpp>
#include "StaticLibEmbed.hpp"


// exported function
extern "C" // avoid name mangling
__attribute__((visibility("default")))
void staticlibembed_public () {
    printf("  staticlibembed_public called.\n");

    // call embedded static library function
    mystaticlib_public();
}
