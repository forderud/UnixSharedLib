echo Cleaning up...
rm -f *.o *.so mainApp ParseElf

# convert embed_example.txt into an object file to binary embedding
# -z noexecstack
ld -r -b binary embed_example.txt -o embed_example.o

echo ""
echo Building libmylib.so...
g++ -fPIC -fvisibility=hidden -c mylib.cpp -o mylib.o
g++ -shared -o libmylib.so mylib.o embed_example.o

echo Building mainApp...
g++ main.cpp -L. -lmylib -Wl,-rpath=. -o mainApp

echo ""
echo Running mainApp:
./mainApp

echo ""
g++ -g ParseElf.cpp -o ParseElf
./ParseElf libmylib.so _binary_embed_example_txt
