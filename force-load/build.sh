echo Cleaning up...
rm -r build install

mkdir build
cd build

echo Building projects...
cmake ..
cmake --build .

echo Installing projects...
cmake --install . --prefix ../install

echo Running MyExecutable...
MyExecutable/MyExecutable
