#include <cstdio>

// exported function
extern "C" // avoid name mangling
__attribute__((visibility("default")))
void innerstaticlib_public () {
    printf("  innerstaticlib_public called.\n");
}
