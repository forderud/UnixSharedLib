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

    printf("Calling MySharedLib function...\n");
#ifdef USE_DLOPEN
    // dynamically load shared library at runtime
  #ifdef __APPLE__
    #if TARGET_OS_IPHONE
      void* handle = dlopen("MySharedLib.framework/MySharedLib", RTLD_LAZY);
    #else
      void* handle = dlopen("MySharedLib/Debug/MySharedLib.framework/MySharedLib", RTLD_LAZY);
    #endif
  #else
    void* handle = dlopen("libMySharedLib.so", RTLD_LAZY);
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
    print_func("embed_example");
    dlclose(handle);
#else
    // call function in shared library directly
    print_embedded_file("embed_example");
#endif

#ifdef __ANDROID__
    return;
#else
    return 0;
#endif
}
