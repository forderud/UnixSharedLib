echo Cleaning up...
rm -f *.o *.a *.so
rm -r *.dSYM
rm -f mainApp parse_mach

echo ""
echo Building libmylib.so...
g++ -fPIC -c mylib.cpp -o mylib.o
# NOTE: section names cannot be longer than 16 chars
ld  mylib.o -r -sectcreate __TEXT embed_example embed_example.txt -o mylib2.o
g++ -shared -fvisibility=default -o libmylib.so mylib2.o

echo ""
echo Building application that links in the shared library and accesses the embedded file...
g++ main.cpp -L. -lmylib -o mainApp

echo ""
echo Running application:
./mainApp

echo ""
g++ -g parse_mach.cpp -o parse_mach
./parse_mach libmylib.so
