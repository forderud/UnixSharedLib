#include <android_native_app_glue.h>

void android_main(struct android_app* state) {
    // Prevent the glue from being stripped by the linker
    app_dummy();

    while (1) {
        int ident;
        int events;
        struct android_poll_source* source;

        // Process system events (lifecycle, input, etc.)
        while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0) {
            if (source != nullptr) {
                source->process(state, source);
            }
            // Check if the app is exiting
            if (state->destroyRequested != 0) {
                return;
            }
        }
        // YOUR RENDER/LOGIC LOOP HERE
    }
}
