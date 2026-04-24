echo Cleaning up...
rm -f *.o *.so *.dylib

echo ""
echo Building MyDynFramework.dylib...

# -fPIC set by default with dynamiclib
g++ -fvisibility=hidden -c MyDynFramework.cpp -o MyDynFramework.o

if [ "$(uname)" = "Darwin" ]; then
  # NOTE: section names cannot be longer than 16 chars
  ld  MyDynFramework.o -r -sectcreate __TEXT embed_example embed_example.txt -o MyDynFramework.o
else
  # convert embed_example.txt into an object file to binary embedding
  # -z noexecstack
  ld -r -b binary embed_example.txt -o embed_example.o
fi

g++ -dynamiclib -o MyDynFramework.dylib MyDynFramework.o
