echo Cleaning up...
rm -f *.o *.so *.dylib

echo ""
echo Building MyDynFramework.dylib...

# -fPIC set by default with dynamiclib
g++ -fPIC -fvisibility=hidden -c MyDynFramework.cpp -o MyDynFramework.o

# NOTE: section names cannot be longer than 16 chars
ld  MyDynFramework.o -r -sectcreate __TEXT embed_example embed_example.txt -o MyDynFramework.o

g++ -dynamiclib -o MyDynFramework.dylib MyDynFramework.o
