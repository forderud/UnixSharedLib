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

#ifdef USE_DLOPEN
    #include <dlfcn.h>
#else
    #include <MySharedLib/MySharedLib.hpp>
#endif


#ifdef __APPLE__
static std::string GetBundleFrameworksDir() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyPrivateFrameworksURL(mainBundle);
    char path[PATH_MAX] {};
    if (CFURLGetFileSystemRepresentation(url, true, (UInt8*)path, sizeof(path))) {
        CFRelease(url);
        return path;
    }
    CFRelease(url);
    abort();
}
#endif

#ifdef __ANDROID__
static std::string GetNativeLibraryDir(ANativeActivity& activity) {
    JNIEnv* env = nullptr;
    activity.vm->AttachCurrentThread(&env, nullptr);
    std::string result;
    {
        // get ApplicationInfo object
        // DOC: https://developer.android.com/reference/android/content/pm/ApplicationInfo
        jclass actCls = env->GetObjectClass(activity.clazz);
        jmethodID getAppInfo = env->GetMethodID(actCls, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
        jobject appInfo = env->CallObjectMethod(activity.clazz, getAppInfo);

        // access nativeLibraryDir field
        jclass aiCls = env->GetObjectClass(appInfo);
        jfieldID fid = env->GetFieldID(aiCls, "nativeLibraryDir", "Ljava/lang/String;");
        auto jstr = (jstring)env->GetObjectField(appInfo, fid);

        const char* buffer = env->GetStringUTFChars(jstr, nullptr);
        result = buffer;
        env->ReleaseStringUTFChars(jstr, buffer);
    }
    activity.vm->DetachCurrentThread();
    return result;
}
#endif


static void LoadLibAndCallFunction(std::filesystem::path libPath) {
#ifdef USE_DLOPEN
    printf("Loading MySharedLib using dlopen...\n");
    std::string path = libPath.filename();
  #ifdef __APPLE__
    path += "/";
    path += libPath.stem(); // append filename without extension
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
  #else
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
  #endif
    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        abort();
    }
    auto print_func = (void (*)(const char*))dlsym(handle, "print_embedded_file");
    if (!print_func) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        abort();
    }
    printf("Calling print_embedded_file function through funtion pointer:\n");
    print_func("embed_example");
    dlclose(handle);
#else
    printf("Calling print_embedded_file function directly:\n");
    print_embedded_file("embed_example");
#endif
}


#ifdef __APPLE__
int main() {
    printf("Hello from MyExecutable!\n");

    printf("\n");
    printf("Frameworks embedded in app bundle:\n");
    std::string libDir = GetBundleFrameworksDir();
    if (!std::filesystem::is_directory(libDir)) {
        printf("(no Frameworks directory)\n");
    } else {
        for (const auto entry : std::filesystem::directory_iterator(libDir)) {
            printf("- %s\n", entry.path().c_str());
            std::string path = entry.path().string();
            path += "/";
            path += entry.path().stem(); // append filename without extension
            FileMap file(path.c_str());
            FindSegmentInFile(file, LibMetadata_SYMBOL_NAME);

            LoadLibAndCallFunction(entry.path());
        }
    }

    return 0;
}
#endif

#ifdef __ANDROID__
void android_main(android_app* state) {
    printf("Hello from MyExecutable!\n");

    printf("Libraries embedded in app bundle:\n");
    ANativeActivity* activity = state->activity;
    AAssetManager* assetManager = activity->assetManager;
    assert(assetManager);
    std::string dataDir = activity->internalDataPath;
    printf("APK data directory: %s\n", dataDir.c_str());
    // TODO: Discover and parse shared libs in app bundle

    std::string libDir = GetNativeLibraryDir(*activity);
    printf("Native lib dir: %s\n", libDir.c_str());
    for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
        printf("* %s\n", entry.path().c_str());
        FileMap file(entry.path().c_str());
        std::string_view data = FindDataSectionInFile(file.ptr(), LibMetadata_SYMBOL_NAME);
        if (!data.empty()) {
            printf("   - Found embedded metadata!\n");
            auto* metadata = (const LibMetadataT*)data.data();
            metadata->Print();

            LoadLibAndCallFunction(entry.path());
        }
    }
}
#endif
