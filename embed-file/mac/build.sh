echo Cleaning up...
rm -r build

mkdir build
cd build

cmake ..
cmake --build .

echo Running MyExecutable...
MyExecutable/MyExecutable

echo ""
ParseMach/ParseMach MyDynFramework/MyDynFramework.framework/MyDynFramework embed_example
ParseMach/ParseMach MyDynFramework/MyDynFramework.framework/MyDynFramework LibMetadata
