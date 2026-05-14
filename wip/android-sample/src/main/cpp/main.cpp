#include <android_native_app_glue.h>
#include <android/log.h>


extern "C"
void android_main(android_app* state) {
    __android_log_print(ANDROID_LOG_INFO, "NativeApp", "App startup.\n");

    while (1) {
        int ident = 0;
        int events = 0;
        android_poll_source* source = nullptr;

        // Process system events (lifecycle, input, etc.)
        while ((ident = ALooper_pollOnce(0, nullptr, &events, (void**)&source)) >= 0) {
            if (source) {
                source->process(state, source);
            }

            if (state->destroyRequested) {
                // app is exiting
                __android_log_print(ANDROID_LOG_INFO, "NativeApp", "App exit.\n");
                return;
            }
        }

        // YOUR RENDER/LOGIC LOOP HERE
    }
}
