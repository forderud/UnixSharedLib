#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
    #include "../ParseMach/ParseMach.hpp"
#endif
#ifdef __ANDROID__
    #include <android_native_app_glue.h>
    #include <jni.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
    #include "../ParseELF/ParseElf.hpp"
#endif
#ifdef __ANDROID__
  #include <android/log.h>
  #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "MySharedLib", __VA_ARGS__)
#else
    #include <stdio.h>
#endif
#include <filesystem>
#include <MySharedLib/MySharedLib.hpp>


#ifdef __APPLE__
static std::string GetBundleFrameworksPath() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyPrivateFrameworksURL(mainBundle);
    char path[1024] {};
    if (CFURLGetFileSystemRepresentation(url, true, (UInt8 *)path, 1024)) {
        CFRelease(url);
        return std::string(path);
    }
    CFRelease(url);
    return "";
}
#endif

#ifdef __ANDROID__
static void openAsset(JNIEnv* env, jobject assetManager, const char* filename) {
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
    if (!asset)
        return;

    size_t size = AAsset_getLength(asset);
    std::vector<char> buffer(size);
    AAsset_read(asset, buffer.data(), size);
    AAsset_close(asset);

    // Process asset data here
}
#endif


#ifdef __ANDROID__
void android_main(android_app* state)
#else
int main()
#endif
{
    printf("Hello from MyExecutable!\n");

    // call function in static library
    print_embedded_file("embed_example");

#ifdef __APPLE__
    printf("\n");
    printf("Frameworks embedded in app bundle:\n");
    std::string path = GetBundleFrameworksPath();
    if (!std::filesystem::is_directory(path)) {
        printf("(no Frameworks directory)\n");
    } else {
        for (const auto entry : std::filesystem::directory_iterator(path)) {
            printf("- %s\n", entry.path().filename().c_str());
            std::string path = entry.path().string();
            path += "/";
            path += entry.path().stem(); // filename without extension
            FileMap file(path.c_str());
            FindSegmentInFile(file, LibMetadata_SYMBOL_NAME);
        }
    }
    printf("\n");
#endif

#ifdef __ANDROID__
    printf("Libraries embedded in app bundle:\n");
    ANativeActivity* activity = state->activity;
    std::string libDir = activity->internalDataPath;
    printf("Lib directory: %s\n", libDir.c_str());
    // TODO: Discover and parse shared libs in app bundle
#endif

    return;
}
