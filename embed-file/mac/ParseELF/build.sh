echo Cleaning up...
rm -f *.o *.so mainApp ParseElf

# convert embed_example.txt into an object file to binary embedding
# -z noexecstack
ld -r -b binary embed_example.txt -o embed_example.o

echo ""
echo Building libmylib.so...
REMOVE_UNREF="-ffunction-sections -fdata-sections -Wl,--gc-sections"
g++ -fPIC -fvisibility=hidden $REMOVE_UNREF -c mylib.cpp -o mylib.o
g++ -shared -o libmylib.so mylib.o embed_example.o
