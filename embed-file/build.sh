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
  ParseELF/ParseELF MyDynFramework/libMyDynFramework.so _binary_embed_example_txt
  ParseELF/ParseELF MyDynFramework/libMyDynFramework.so LibMetadata
fi
