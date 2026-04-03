echo Cleaning up...
rm -f *.o *.a *.so
rm -r *.dSYM
rm -f mainApp parse_mach

echo Building static library that embeds a binary file...
g++ -c mystaticlib.cpp -o lib_sources.o
# NOTE: section names cannot be longer than 16 chars
ld  lib_sources.o -r -sectcreate __TEXT embed_example embed_example.txt -o mystaticlib.o
libtool -static mystaticlib.o -o libmystatic.a

echo ""
echo Building libmylib.so...
g++ -fPIC -c mylib.cpp -o mylib.o
g++ -shared -fvisibility=default -o libmylib.so mylib.o libmystatic.a

echo ""
echo Building application that links in the static library and accesses the embedded file...
g++ main.cpp -L. -lmystatic -lmylib -o mainApp

echo ""
echo Running application:
./mainApp

echo ""
g++ -g parse_mach.cpp -o parse_mach
#./parse_mach libmylib.so
./parse_mach mainApp
