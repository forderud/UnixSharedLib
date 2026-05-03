echo Cleaning up...
rm -f *.o *.a *.so mainApp

# Doc: https://gcc.gnu.org/wiki/Visibility
compile_flags="-fvisibility=hidden -fvisibility-inlines-hidden -fPIC"

echo ""
echo Building static lib...
g++ $compile_flags -c mystaticlib.cpp -o mystaticlib.o
ar rcs libmystaticlib.a mystaticlib.o

#echo libmystaticlib.a imports:
#nm libmystaticlib.a

echo ""
echo Building shared lib...
g++ $compile_flags -c mysharedlib.cpp -o mysharedlib.o
# passing -static-libgcc -static-libstdc++ to avoid run-time dependencies to C/C++ libraries
# passing -Wl,--exclude-libs,ALL to avoid exposing 3rd party lib. symbols
g++ $compile_flags -shared -static-libgcc -static-libstdc++ mysharedlib.o libmystaticlib.a -o libmysharedlib.so -Wl,--exclude-libs,ALL

echo Transitive shared lib. dependencies:
ldd libmysharedlib.so
#echo Direct shared lib. dependencies:
#readelf -d libmysharedlib.so

echo shared lib. exports:
readelf --symbols --use-dynamic --demangle --wide libmysharedlib.so 

echo Building application...
g++ main.cpp -L. -lmysharedlib -Wl,-rpath=. -o mainApp

#echo mainApp dependencies:
#ldd mainApp

echo ""
echo Running application...
./mainApp
