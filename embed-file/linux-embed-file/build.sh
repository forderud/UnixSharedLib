echo Cleaning up...
rm -f *.o *.so mainApp parse_elf

# convert embed_example.txt into an object file to binary embedding
# -z noexecstack
ld -r -b binary embed_example.txt -o embed_example.o

echo ""
echo Building libmylib.so...
g++ -fPIC -c mylib.cpp -o mylib.o
g++ -shared -fvisibility=default -o libmylib.so mylib.o embed_example.o

echo Building mainApp...
g++ main.cpp -L. -lmylib -Wl,-rpath=. -o mainApp

echo ""
echo Running mainApp:
./mainApp

echo ""
g++ -g parse_elf.cpp -o parse_elf
./parse_elf libmylib.so _binary_embed_example_txt
