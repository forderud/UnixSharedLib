#include <cassert>
#include <filesystem>
#include <dlfcn.h>
#include <CoreFoundation/CoreFoundation.h>


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

/** macOS & iOS entry point */
int main(int argc, char *argv[]) {
    std::string libDir = GetBundleFrameworksDir();
    printf("Frameworks dir: %s\n", libDir.c_str());

    printf("\nFrameworks embedded in app bundle:\n");
    for (const auto& entry : std::filesystem::directory_iterator(libDir)) {
        printf("* %s\n", entry.path().filename().c_str());

        std::string libFilename = entry.path().filename();
        libFilename += "/";
        libFilename += entry.path().stem(); // filename without extension

        void* handle = dlopen(libFilename.c_str(), RTLD_LAZY);
        assert(handle);
        // TODO: call dlsym to access exported functions and call them
    }

    return 0;
}
