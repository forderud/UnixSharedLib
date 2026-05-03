set -e   # stop on first failure

ARG=$1 # "" or "iOS"

echo Cleaning up...
rm -rf build install test

if [ "$ARG" = "iOS" ]; then
    echo Building for iOS
    # Open "Keychain Access". Select "login" keychain, filter by "Certificates" and look for a "Apple. Development: <your name>"  certificate. The 10-character development team code is found in the "Organizational Unit" attribute.
    #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM="<TODO>"
    CONFIG="--config Debug"
    PLATFORM="-GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$XCODE_ATTRIBUTE_DEVELOPMENT_TEAM -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH"
    BUILD_PARAMS="-- -allowProvisioningUpdates"
elif [ "$ARG" = "Android" ]; then
    echo WIP: Building for Android
    CONFIG="--config Debug"
    PLATFORM="-DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK=$HOME/Library/Android/sdk/ndk/30.0.14904198 -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a"
    BUILD_PARAMS=""
else
    echo Building for native platform
    CONFIG=""
    PLATFORM=""
    BUILD_PARAMS=""
fi

echo Building projects...
cmake -S . -B build $PLATFORM
cmake --build build $CONFIG $BUILD_PARAMS

echo Installing projects...
cmake --install build $CONFIG --prefix install

if [ "$ARG" = "" ]; then
    echo Running MyExecutable...
    build/MyExecutable/MyExecutable

    echo Building independent test...
    cmake -DCMAKE_PREFIX_PATH="../install" -S IndependentTest -B test $PLATFORM
    cmake --build test $CONFIG

    echo Running independent test...
    test/IndependentTest
fi
