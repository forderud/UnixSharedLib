#!/bin/bash
cd "$(dirname "$0")"
set -e # stop on first failure

echo Cleaning up...
rm -f *.o *.a *.so a.out

echo ""
echo Building static C++ lib...
g++ -fPIC -c mystaticlib.cpp -o mystaticlib.o
ar rcs libmystaticlib.a mystaticlib.o

#echo libmystaticlib.a imports:
#nm libmystaticlib.a

echo ""
echo Building shared C++ lib...
g++ -fPIC -c mysharedlib.cpp -o mysharedlib.o
sharedlib_flags="-Wl,-Bsymbolic -Wl,-Bsymbolic-functions" # TODO: Learn how to use these flags
g++ -shared -static-libgcc -static-libstdc++ -Wl,--version-script=mysharedlib.vers $sharedlib_flags -pthread mysharedlib.o libmystaticlib.a -o libmysharedlib.so

echo Transitive shared lib. dependencies:
ldd libmysharedlib.so
echo Direct shared lib. dependencies:
readelf -d libmysharedlib.so

echo Building C++ application...
# Optional define: -DUSE_DLOPEN
g++ main.cpp -L. -ldl -lmysharedlib -Wl,-rpath=.

#echo executable dependencies:
#ldd a.out

echo ""
echo Running application...
./a.out
