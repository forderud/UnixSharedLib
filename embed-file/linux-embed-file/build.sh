echo Cleaning up...
rm -f *.o mainApp

# convert embed_example.txt into an object file to binary embedding
ld -r -b binary embed_example.txt -o embed_example.o 

echo Building mainApp...
g++ main.cpp embed_example.o -o mainApp

echo ""
echo Running mainApp:
./mainApp

echo "Parsing mainApp symbols..."
g++ parse_elf.cpp -o parse_elf
./parse_elf mainApp
