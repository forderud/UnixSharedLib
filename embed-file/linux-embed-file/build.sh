echo Cleaning up...
rm -f *.o *.so mainApp parse_elf

# convert embed_example.txt into an object file to binary embedding
# -z start-stop-visibility=default (default, internal, hidden, or protected)
ld -r -b binary embed_example.txt -z start-stop-visibility=default -z noexecstack -o embed_example.o

echo ""
echo Building libmylib.so...
g++ -fPIC -c mylib.cpp -o mylib.o
# -fvisibility=default
# -Wl,--version-script=mylib.map
g++ -shared -o libmylib.so mylib.o

echo Building mainApp...
g++ main.cpp -L. -lmylib -Wl,-rpath=. embed_example.o -o mainApp

echo ""
echo Running mainApp:
./mainApp

echo ""
g++ -g parse_elf.cpp -o parse_elf
./parse_elf mainApp
