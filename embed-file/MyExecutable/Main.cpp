#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
    #include "../ParseMach/ParseMach.hpp"
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
            path += "/MySharedLib"; // TODO: Avoid hardcoding dylib name
            FileMap file(path.c_str());
            FindSegmentInSegments(file, "LibMetadata");
        }
    }
    printf("\n");
#endif

    return 0;
}
