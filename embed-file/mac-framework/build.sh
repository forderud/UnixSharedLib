echo Cleaning up...
rm -r build

mkdir build
cd build

cmake ..
cmake --build .

MyExecutable/MyExecutable
