#include <android_native_app_glue.h>
#include <android/log.h>


static void handle_cmd(android_app* /*app*/, int32_t cmd) {
    switch (cmd) {
    case APP_CMD_RESUME:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Entered foreground (RESUME).");
        break;
    case APP_CMD_PAUSE:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Entered background (PAUSE).");
        break;
    case APP_CMD_START:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Activity visible (START).");
        break;
    case APP_CMD_STOP:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Activity hidden (STOP).");
        break;
    case APP_CMD_GAINED_FOCUS:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Window gained focus.");
        break;
    case APP_CMD_LOST_FOCUS:
        __android_log_print(ANDROID_LOG_INFO, "NativeApp", "Window lost focus.");
        break;
    }
}

extern "C"
void android_main(android_app* state) {
    __android_log_print(ANDROID_LOG_INFO, "NativeApp", "App startup.\n");

    state->onAppCmd = handle_cmd;

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
