set -e   # stop on first failure

ARG=$1 # "", "iOS" or "Android"

echo Cleaning up...
rm -rf build install
rm -rf .cxx .gradle

if [ "$ARG" = "iOS" ]; then
    echo Building for iOS
    #XCODE_ATTRIBUTE_DEVELOPMENT_TEAM="<TODO>"
    CONFIG="--config Debug"
    PLATFORM="-GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$XCODE_ATTRIBUTE_DEVELOPMENT_TEAM -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH"
    BUILD_PARAMS="-- -allowProvisioningUpdates"
elif [ "$ARG" = "Android" ]; then
    echo Building for Android
    #ANDROID_NDK_ROOT=$HOME/Library/Android/sdk/ndk/<version>
    ./gradlew assembleDebug
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

if [ "$ARG" != "Android" ]; then
    cmake -S . -B build $PLATFORM
    cmake --build build $CONFIG $BUILD_PARAMS
fi

if [ "$ARG" = "" ]; then
    cmake --install build --config Debug --prefix install

    cd install

    echo Running MobileDlopen...
    if [ "$(uname)" = "Darwin" ]; then
      MobileDlopen.app/Contents/MacOS/MobileDlopen
    else
      ./MobileDlopen
    fi

    echo ""
    if [ "$(uname)" = "Darwin" ]; then
        echo "Running ParseMach..."
        ./ParseMach MySharedLib.framework/MySharedLib embed_example
        ./ParseMach MySharedLib.framework/MySharedLib LibMetadata
    else
        echo "Running ParseELF..."
        ./ParseELF libMySharedLib.so _binary_embed_example_txt
        ./ParseELF libMySharedLib.so LibMetadata
    fi
fi
