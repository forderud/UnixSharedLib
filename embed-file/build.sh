set -e   # stop on first failure

ARG=$1 # "", "iOS" or "Android"

echo Cleaning up...
rm -rf build

if [ "$ARG" = "iOS" ]; then
    echo Building for iOS
    # Open "Keychain Access". Select "login" keychain, filter by "Certificates" and look for a "Apple. Development: <your name>"  certificate. The 10-character development team code is found in the "Organizational Unit" attribute.
    #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM="<TODO>"
    CONFIG="--config Debug"
    PLATFORM="-GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$XCODE_ATTRIBUTE_DEVELOPMENT_TEAM -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH"
    BUILD_PARAMS="-- -allowProvisioningUpdates"
elif [ "$ARG" = "Android" ]; then
    echo Building for Android
    #ANDROID_NDK=$HOME/Library/Android/sdk/ndk/<version>
    CONFIG=""
    PLATFORM="-DCMAKE_BUILD_TYPE=Debug -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_NDK=$ANDROID_NDK -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH"
    BUILD_PARAMS=""
else
    echo Building for native platform
    CONFIG=""
    if [ "$(uname)" = "Darwin" ]; then
      PLATFORM="-GXcode"
    else
      PLATFORM=""
    fi
    BUILD_PARAMS=""
fi

cmake -S . -B build $PLATFORM
cmake --build build $CONFIG $BUILD_PARAMS

cd build

if [ "$ARG" = "" ]; then
    echo Running MyExecutable...
    if [ "$(uname)" = "Darwin" ]; then
      MyExecutable/Debug/MyExecutable.app/Contents/MacOS/MyExecutable
    else
      MyExecutable/MyExecutable
    fi

    echo ""
    if [ "$(uname)" = "Darwin" ]; then
        ParseMach/Debug/ParseMach MySharedLib/Debug/MySharedLib.framework/MySharedLib embed_example
        ParseMach/Debug/ParseMach MySharedLib/Debug/MySharedLib.framework/MySharedLib LibMetadata
    else
        ParseELF/ParseELF MySharedLib/libMySharedLib.so _binary_embed_example_txt
        ParseELF/ParseELF MySharedLib/libMySharedLib.so LibMetadata
    fi
fi

if [ "$ARG" = "iOS" ]; then
    echo Running MyExecutable on iOS Simulator...
    #xcrun simctl install booted MyExecutable/Debug-iphoneos/MyExecutable.app
    #xcrun simctl launch booted com.yourcompany.MyExecutable
fi
