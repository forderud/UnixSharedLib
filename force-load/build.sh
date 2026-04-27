set -e   # stop on first failure

echo Cleaning up...
rm -rf build install test

echo Building projects...
cmake -S . -B build
cmake --build build

echo Installing projects...
cmake --install build --prefix install

echo Running MyExecutable...
build/MyExecutable/MyExecutable

echo Building independent test...
cmake -DCMAKE_PREFIX_PATH="../install" -S IndependentTest -B test
cmake --build test

echo Running independent test...
test/IndependentTest
