#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif
#include <iostream>
#include <filesystem>
#include <MySharedLib/MySharedLib.hpp>


#ifdef __APPLE__
static std::string get_bundle_frameworks_path() {
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
    std::string path = get_bundle_frameworks_path();
    printf("Listing bundle frameworks in %s:\n", path.c_str());
    if (path.empty() || !std::filesystem::is_directory(path)) {
        std::cout << "(no Frameworks directory)\n";
    } else {
        for (const auto entry : std::filesystem::directory_iterator(path)) {
            std::cout << entry.path().filename() << std::endl;
        }
    }
#endif

    return 0;
}
