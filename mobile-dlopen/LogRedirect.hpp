#pragma once
// printf wrappers for Android and iOS log redirection

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
        // route printf to both the unified system log and the on-screen UITextView.
        #include <os/log.h>
        #include <stdarg.h>
        #include <string.h>
        extern "C" void iOSLogAppend(const char* msg); // defined in iOSApp.mm
        static inline void ios_log_printf(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            char buf[1024];
            vsnprintf(buf, sizeof(buf), fmt, args);
            va_end(args);
            // Append to on-screen log (keep trailing newline for readability).
            iOSLogAppend(buf);
            // Strip trailing newline before forwarding to os_log (it adds one).
            size_t n = strlen(buf);
            if (n && buf[n-1] == '\n') buf[n-1] = '\0';
            os_log(OS_LOG_DEFAULT, "%{public}s", buf);
        }
        #define printf(...) ios_log_printf(__VA_ARGS__)
    #endif
#endif
