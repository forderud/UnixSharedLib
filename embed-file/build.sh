set -e   # stop on first failure

echo Cleaning up...
rm -rf build

cmake -S . -B build
cmake --build build

cd build

echo Running MyExecutable...
MyExecutable/MyExecutable

echo ""
if [ "$(uname)" = "Darwin" ]; then
  ParseMach/ParseMach MySharedLib/MySharedLib.framework/MySharedLib embed_example
  ParseMach/ParseMach MySharedLib/MySharedLib.framework/MySharedLib LibMetadata
else
  ParseELF/ParseELF MySharedLib/libMySharedLib.so _binary_embed_example_txt
  ParseELF/ParseELF MySharedLib/libMySharedLib.so LibMetadata
fi
