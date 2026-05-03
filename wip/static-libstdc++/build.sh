#!/bin/bash
cd "$(dirname "$0")"
set -e # stop on first failure

echo Cleaning up...
rm -f *.so a.out

echo "Building shared lib..."
# Link to static libstdc++ to avoid run-time dependency.
# Use --exclude-libs,ALL to avoid exporting symbols from  libstdc++ and other dependent libraries
g++ -fPIC -static-libstdc++ -shared libfoo.cpp -o libfoo.so -Wl,--exclude-libs,ALL -pthread

echo "Building app that links to shared lib..."
g++ main.cpp -L. -lfoo -Wl,-rpath=. -pthread -O2


echo ""
echo "Running application (will segfault):"
./a.out
