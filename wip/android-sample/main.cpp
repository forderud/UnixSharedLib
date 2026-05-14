#include <android_native_app_glue.h>
#include "MySharedLib.hpp"

#ifdef __ANDROID__
  #include <android/log.h>
  #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "NativeApp", __VA_ARGS__)
#else
    #include <stdio.h>
#endif

static void handle_cmd(android_app* /*app*/, int32_t cmd) {
    switch (cmd) {
    case APP_CMD_RESUME:
        printf("Entered foreground (RESUME).");
        break;
    case APP_CMD_PAUSE:
        printf("Entered background (PAUSE).");
        break;
    case APP_CMD_START:
        printf("Activity visible (START).");
        break;
    case APP_CMD_STOP:
        printf("Activity hidden (STOP).");
        break;
    case APP_CMD_GAINED_FOCUS:
        printf("Window gained focus.");
        break;
    case APP_CMD_LOST_FOCUS:
        printf("Window lost focus.");
        break;
    }
}

extern "C"
void android_main(android_app* state) {
    printf("App startup.\n");

    MySharedLib_function();

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
                printf("App exit.\n");
                return;
            }
        }

        // YOUR RENDER/LOGIC LOOP HERE
    }
}
