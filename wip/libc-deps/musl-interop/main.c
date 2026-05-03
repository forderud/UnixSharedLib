#include <stdio.h>
#include <pthread.h>
#include "sharedlib.h"

static void* thread_function(void*) {
    printf("Hello from thread!\n");
    return NULL;
}

int main () {
    //printf("executable built against glibc %d.%d\n", __GLIBC__, __GLIBC_MINOR__);

    printf("Calling compute_sum...\n");
    int sum = compute_sum(3, 4);
    printf("Returned %i\n", sum);

    pthread_t thread = {};
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);

    return 0;
}
