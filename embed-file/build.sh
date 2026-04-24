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
  ParseMach/ParseMach MySharedLib/MyDynFramework.framework/MyDynFramework embed_example
  ParseMach/ParseMach MySharedLib/MyDynFramework.framework/MyDynFramework LibMetadata
else
  ParseELF/ParseELF MySharedLib/libMyDynFramework.so _binary_embed_example_txt
  ParseELF/ParseELF MySharedLib/libMyDynFramework.so LibMetadata
fi
