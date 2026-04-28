#include <cstdio>
#ifndef __EMSCRIPTEN__
    #include <MySharedLib.hpp>
#endif
#include <MyStaticLib.hpp>


int main() {
    printf("Program startup.\n");

#ifndef __EMSCRIPTEN__
    //mysharedlib_init();
    //mysharedlib_public();
#endif
    //mystaticlib_init();
    //mystaticlib_public();

    return 0;
}
