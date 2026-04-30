#include <cstdio>
#include <MyStaticLib.hpp>

// exported function
extern "C" // avoid name mangling
__attribute__((visibility("default")))
void staticlibwrap_public () {
    printf("  staticlibwrap_public called.\n");

    mystaticlib_public();
}
