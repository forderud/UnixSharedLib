#!/bin/bash
cd "$(dirname "$0")"
set -e # stop on first failure

# Using "musl-gcc" instead of "gcc" to use the musl C library instead of glibc
# Musl seem to be incompatible with "ldd", so it cannot be used for analyzing dependencies

echo Cleaning up...
rm -f *.o *.a *.so mainApp

echo Building shared C library..
# Use musl in shared library
musl-gcc -shared sharedlib.c -o libsharedLib.so

echo Direct shared lib. dependencies:
readelf -d libsharedlib.so

# Add musl to LD_LIBRARY_PATH to avoid "invalid ELF header" errors when combining glibc with musl in the same process 
export LD_LIBRARY_PATH=/lib/x86_64-linux-musl:$LD_LIBRARY_PATH # required for glibc/musl interop

echo Building C application...
# Use glibc in executable to test interop with shared lib. using musl
gcc main.c -L. -pthread -lsharedlib -o mainApp

echo ""
echo Running C application...
./mainApp
