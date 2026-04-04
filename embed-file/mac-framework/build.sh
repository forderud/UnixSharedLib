echo Cleaning up...
rm -f *.o *.a *.so *.dylib
rm -r *.dSYM
rm -f mainApp parse_mach

cmake .
cmake --build .
