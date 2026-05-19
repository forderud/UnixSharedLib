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
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        // route printf to the unified system log
        #include <os/log.h>
        #include <stdarg.h>
        static inline void ios_log_printf(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            char buf[1024];
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            // Strip trailing newline (os_log adds its own).
            size_t n = strlen(buf);
            if (n && buf[n-1] == '\n') buf[n-1] = '\0';
            os_log(OS_LOG_DEFAULT, "%{public}s", buf);
        }
        #define printf(...) ios_log_printf(__VA_ARGS__)
    #endif
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
        std::string path = entry.path();
        path += "/";
        path += entry.path().stem(); // append filename without extension
        FileMap file(path.c_str());
        std::string_view data = FindSegmentInFile(file, LibMetadata_SYMBOL_NAME);
        if (data.empty()) {
            printf("No embedded metadata found.\n");
            continue;
        }

        auto* metadata = (const LibMetadataT*)(data.data());
        metadata->Print();

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
        FileMap file(entry.path().c_str());
        std::string_view data = FindDataSectionInFile(file.ptr(), LibMetadata_SYMBOL_NAME);
        if (data.empty()) {
            printf("No embedded metadata found.\n");
            continue;
        }

        auto* metadata = (const LibMetadataT*)data.data();
        metadata->Print();

        LoadLibAndCallFunction(entry.path());
    }
}
#endif
