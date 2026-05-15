#include <cassert>
#include "../FileMap.hpp"
#ifdef __ANDROID__
  #include <android/log.h>
  #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "MySharedLib", __VA_ARGS__)
#else
    #include <stdio.h>
#endif
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

static void traverseAssets(AAssetDir* assetDir) {
    const char* fileName = nullptr; // weak ptr.
    // getNextFileName returns the relative path of files inside the directory
    while ((fileName = AAssetDir_getNextFileName(assetDir))) {
        printf("* Found file: %s\n", fileName);
    }
}

static std::string GetNativeLibraryDir(ANativeActivity* activity) {
    JNIEnv* env = nullptr;
    activity->vm->AttachCurrentThread(&env, nullptr);

    jclass actCls       = env->GetObjectClass(activity->clazz);
    jmethodID getAppInfo = env->GetMethodID(actCls, "getApplicationInfo",
                                            "()Landroid/content/pm/ApplicationInfo;");
    jobject appInfo     = env->CallObjectMethod(activity->clazz, getAppInfo);

    jclass aiCls        = env->GetObjectClass(appInfo);
    jfieldID fid        = env->GetFieldID(aiCls, "nativeLibraryDir", "Ljava/lang/String;");
    auto jstr           = (jstring)env->GetObjectField(appInfo, fid);

    const char* c = env->GetStringUTFChars(jstr, nullptr);
    std::string result = c;
    env->ReleaseStringUTFChars(jstr, c);

    activity->vm->DetachCurrentThread();
    return result;
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
    AAssetManager* assetManager = activity->assetManager;
    assert(assetManager);
    std::string dataDir = activity->internalDataPath;
    printf("APK data directory: %s\n", dataDir.c_str());
    // TODO: Discover and parse shared libs in app bundle

    {
        // list files in "assets" folder (not possible to traverse lib/arm64-v8a this way)
        AAssetDir* assetDir = AAssetManager_openDir(assetManager, "");
        assert(assetDir);
        traverseAssets(assetDir);
        AAssetDir_close(assetDir);
        printf("Asset listing completed.\n");
    }

    std::string libDir = GetNativeLibraryDir(activity);
    printf("Native lib dir: %s\n", libDir.c_str());
    for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
        printf("* %s\n", entry.path().filename().c_str());
        FileMap file(entry.path().c_str());
        std::string_view data = FindDataSectionInFile(file.ptr(), LibMetadata_SYMBOL_NAME);
        if (!data.empty()) {
            printf("   - Found embedded metadata!\n");
            auto* metadata = (const LibMetadataT*)data.data();
            metadata->Print();
        }
    }
#endif

#ifdef __ANDROID__
    return;
#else
    return 0;
#endif
}
