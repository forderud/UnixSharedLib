#include <cassert>
#include "../FileMap.hpp"
#include "../LogRedirect.hpp"
#ifdef __APPLE__
    #include "../ParseMach/ParseMach.hpp"
#endif
#ifdef __ANDROID__
    #include <android_native_app_glue.h>
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

/** Get path to dylib inside framework */
static std::string DylibPath(std::filesystem::path frameworkPath, bool fullPath) {
    std::string path = fullPath ? frameworkPath : frameworkPath.filename();
    path += "/";
    path += frameworkPath.stem(); // filename without extension
    return path;
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
    printf("Loading and calling shared lib using dlopen:\n");
  #ifdef __APPLE__
    std::string path = DylibPath(libPath, false); // remove path prefix
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
  #else
    std::string path = libPath.filename(); // remove path prefix
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
  #endif
    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        abort();
    }
    auto lib_func = (std::string_view (*)(const char*))dlsym(handle, "LibraryFunction");
    if (!lib_func) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        abort();
    }
    // calling LibraryFunction function through function pointer
    auto data = lib_func("embed_example");
    printf("%.*s\n", (int)data.size(), data.data());
    dlclose(handle);
#else
    printf("Calling LibraryFunction function directly:\n");
    auto data = LibraryFunction("embed_example");
    printf("%.*s\n", (int)data.size(), data.data());
#endif
}


#ifdef __APPLE__
int main(int argc, char *argv[]) {
    std::string libDir = GetBundleFrameworksDir();
    if (!std::filesystem::is_directory(libDir)) {
        printf("ERROR: Frameworks directory not found\n");
        abort();
    }
    printf("Frameworks dir: %s\n", libDir.c_str());

    printf("\nFrameworks embedded in app bundle:\n");
    for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
        printf("\n## %s\n", entry.path().filename().c_str());
        std::string path = DylibPath(entry.path(), true); // keep path prefix
        FileMap file(path.c_str());
        std::string_view data = FindSegmentInFile(file, LibMetadata_SYMBOL_NAME);
        if (data.empty()) {
            printf("No embedded metadata found.\n");
            continue;
        }

        auto* metadata = (const LibMetadataT*)(data.data());
        metadata->Print();
        printf("\n");

        LoadLibAndCallFunction(entry.path());
    }

    return 0;
}
#endif

#ifdef __ANDROID__
void android_main(android_app* state) {
    ANativeActivity* activity = state->activity;
    std::string libDir = GetNativeLibraryDir(*activity);
    printf("Native lib dir: %s\n", libDir.c_str());

    printf("\nLibraries embedded in app bundle:\n");
    for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
        printf("\n## %s\n", entry.path().filename().c_str());
        FileMap file(entry.path().c_str()); // full path
        std::string_view data = FindDataSectionInFile(file.ptr(), LibMetadata_SYMBOL_NAME);
        if (data.empty()) {
            printf("No embedded metadata found.\n");
            continue;
        }

        auto* metadata = (const LibMetadataT*)data.data();
        metadata->Print();
        printf("\n");

        LoadLibAndCallFunction(entry.path());
    }
}
#endif
