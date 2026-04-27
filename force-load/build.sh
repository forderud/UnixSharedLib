echo Cleaning up...
rm -r build

mkdir build
cd build

echo Building projects...
cmake ..
cmake --build .

echo Running MyExecutable...
MyExecutable/MyExecutable
