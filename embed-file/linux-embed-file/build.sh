echo Cleaning up...
rm -f *.o *.so mainApp parse_elf

# convert embed_example.txt into an object file to binary embedding
# -z start-stop-visibility=default (default, internal, hidden, or protected)
ld -r -b binary embed_example.txt -z noexecstack -o embed_example.o
if [[ $(uname -m) == "aarch64" ]]; then
    bfdname="elf64-littleaarch64"
else
    bfdname="elf64-x86-64"
fi
#objcopy --input-target binary --output $bfdname --add-section .note.GNU-stack=/dev/null embed_example.txt embed_example.o

echo ""
echo Building libmylib.so...
g++ -fPIC -c mylib.cpp -o mylib.o
# -fvisibility=default
# -Wl,--version-script=mylib.map
g++ -shared -o libmylib.so mylib.o embed_example.o

echo Building mainApp...
# -no-pie
g++ main.cpp -L. -lmylib -Wl,-rpath=. -o mainApp

echo ""
echo Running mainApp:
./mainApp

echo ""
g++ -g parse_elf.cpp -o parse_elf
./parse_elf libmylib.so _binary_embed_example_txt
