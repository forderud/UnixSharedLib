echo Cleaning up...
rm -r build

mkdir build
cd build

cmake ..
cmake --build .

echo Running MyExecutable...
MyExecutable/MyExecutable

echo ""
if [ "$(uname)" = "Darwin" ]; then
  ParseMach/ParseMach MyDynFramework/MyDynFramework.framework/MyDynFramework embed_example
  ParseMach/ParseMach MyDynFramework/MyDynFramework.framework/MyDynFramework LibMetadata
else
  ParseELF/ParseELF MyDynFramework/libMyDynFramework.so embed_example
  ParseELF/ParseELF MyDynFramework/libMyDynFramework.so LibMetadata
fi
