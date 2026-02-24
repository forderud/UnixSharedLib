#!/bin/bash
cd "$(dirname "$0")"
set -e # stop on first failure

# This script assumes that libstdc++.so.6 and libModernStuff.so are already present in the current dir.

link_flags="-Wl,-rpath=\$ORIGIN"

# Alternative mechanism for using bundled libstdc++ version
#export LD_LIBRARY_PATH=`pwd`

#echo Shared lib. dependencies:
#ldd libModernStuff.so
#readelf -d libModernStuff.so

echo Building C++ application with old compiler with outdated libstdc++...
# Passing -nodefaultlibs to avoid auto-linking to GCC-bundled libstdc++ (/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++.so)
# TODO: Switch to -nostdlib++ after upgrading to GCC 13
# Need to link to static GCC exception handling library (gcc_eh) if using std::ifstream
# Add -Wl,--trace to list linked-to libraries
g++ main.cpp -nodefaultlibs -lgcc_eh -L. -lc `pwd`/libstdc++.so.6 -lModernStuff $link_flags

#echo executable dependencies:
#ldd a.out

echo ""
echo Running application...
./a.out
