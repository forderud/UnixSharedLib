echo Cleaning up...
rm -f *.o *.a *.so *.dylib
rm -r *.dSYM
rm -f mainApp ParseMach

echo ""
echo Building libmylib.so...
# -fPIC set by default with dynamiclib
g++ -fPIC -fvisibility=hidden -c mylib.cpp -o mylib.o
# NOTE: section names cannot be longer than 16 chars
ld  mylib.o -r -sectcreate __TEXT embed_example embed_example.txt -o mylib.o
g++ -dynamiclib -o libmylib.dylib mylib.o

echo ""
echo Building application that links in the shared library and accesses the embedded file...
g++ main.cpp -L. -lmylib -o mainApp

echo ""
echo Running application:
./mainApp
