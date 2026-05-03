#include <future>

extern void foo();

int main() {
    printf("Calling std::async from executable...\n");
    auto result = std::async(std::launch::async, [](){});

    printf("Calling shared lib...\n");
    foo();
}
