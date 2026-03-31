#!/bin/bash
cd "$(dirname "$0")"
set -e # stop on first failure

echo Cleaning up...
rm -f *.o *.a *.so mainApp

# Use non-default glibc
link_flags=""
link_flags="-L/usr/local/glibc-2.26/lib -I/usr/local/glibc-2.26/include -Wl,-rpath=/usr/local/glibc-2.26/lib -Wl,-dynamic-linker=/usr/local/glibc-2.26/lib/ld-linux-x86-64.so.2"

compile_flags=""
#compile_flags="$compile_flags -isystem /usr/local/glibc-2.26/include"

echo Building shared C library..
g++ -shared -fPIC $compile_flags $link_flags -static-libgcc -static-libstdc++ sharedlib.cpp -L. -o libsharedLib.so

echo Transitive shared lib. dependencies:
ldd libsharedlib.so
echo Direct shared lib. dependencies:
readelf -d libsharedlib.so

echo Building C application...
g++ $link_flags main.cpp -L. -pthread -lsharedlib -Wl,-rpath=. -o mainApp

echo mainApp dependencies:
ldd mainApp

echo ""
echo Running C application...
./mainApp
