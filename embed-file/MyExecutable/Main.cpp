#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
    #include "../ParseMach/ParseMach.hpp"
#endif
#ifdef __ANDROID__
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
    #include "../ParseELF/ParseElf.hpp"
#endif
#include <iostream>
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


int main() {
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
            FindSegmentInFile(file, "LibMetadata");
        }
    }
    printf("\n");
#endif

#ifdef __ANDROID__
    // TODO: Parse shared libs in app bundle
#endif

    return 0;
}
