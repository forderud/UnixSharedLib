#include <future>

void foo() {
    printf("Calling std::async from shared lib...\n");
    auto result = std::async(std::launch::async, [](){});
}
