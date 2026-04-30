set -e   # stop on first failure

echo Cleaning up...
rm -rf build install test


# Open "Keychain Access". Select "login" keychain, filter by "Certificates" and look for a "Apple. Development: <your name>"  certificate. The 10-character development team code is found in the "Organizational Unit" attribute.
XCODE_DEVELOPMENT_TEAM="<TODO>"

PLATFORM=""
#PLATFORM="-GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$XCODE_DEVELOPMENT_TEAM"

BUILD_PARAMS=""
#BUILD_PARAMS="-- -allowProvisioningUpdates"

echo Building projects...
cmake -S . -B build $PLATFORM
cmake --build build --config Debug $BUILD_PARAMS

echo Installing projects...
cmake --install build --config Debug --prefix install

echo Running MyExecutable...
build/MyExecutable/MyExecutable

echo Building independent test...
cmake -DCMAKE_PREFIX_PATH="../install" -S IndependentTest -B test $PLATFORM
cmake --build test

echo Running independent test...
test/IndependentTest
