#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif
#include <iostream>
#include <filesystem>
#include <MySharedLib/MySharedLib.hpp>


#ifdef __APPLE__
static std::string get_bundle_resource_path() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[1024] {};
    if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8 *)path, 1024)) {
        CFRelease(resourceURL);
        return std::string(path);
    }
    CFRelease(resourceURL);
    return "";
}
#endif


int main() {
    // call function in static library
    print_embedded_file("embed_example");

#ifdef __APPLE__
    std::string path = get_bundle_resource_path();
    printf("Listing bundle resources in %s:\n", path.c_str());
    if (path.empty() || !std::filesystem::is_directory(path)) {
        std::cout << "(no Resources directory)\n";
    } else {
        for (const auto entry : std::filesystem::directory_iterator(path)) {
            std::cout << entry.path().filename() << std::endl;
        }
    }
#endif

    return 0;
}
