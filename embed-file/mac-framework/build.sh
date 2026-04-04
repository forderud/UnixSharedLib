echo Cleaning up...
rm -f *.o *.a *.so *.dylib
rm -f cmake_install.cmake CMakeCache.txt Makefile
rm -r CMakeFiles
rm -r MyDynFramework.framework

cmake .
cmake --build .
