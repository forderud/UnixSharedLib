echo Cleaning up...
rm -f *.o *.so mainApp

echo ""
echo Building libmylib.so...
REMOVE_UNREF="-ffunction-sections -fdata-sections -Wl,--gc-sections"
g++ -fPIC -fvisibility=hidden $REMOVE_UNREF -c mylib.cpp -o mylib.o
g++ -shared -o libmylib.so mylib.o embed_example.o

echo Building mainApp...
g++ main.cpp -L. -lmylib -Wl,-rpath=. -o mainApp

echo ""
echo Running mainApp:
./mainApp
