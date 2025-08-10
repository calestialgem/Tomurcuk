#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <tomurcuk/base/LinearMemoryAllocator.hpp>
#include <tomurcuk/status/Crashes.hpp>

auto main(int, char **) -> int {
    tomurcuk::LinearMemoryAllocator linearMemoryAllocator;
    linearMemoryAllocator.initialize(1024);

    auto memoryAllocator = linearMemoryAllocator.getMemoryAllocator();

    static constexpr int64_t kCount = 444;
    int64_t *numbers;
    if (!memoryAllocator.allocateArray(&numbers, kCount)) {
        tomurcuk::Crashes::crash("Could not allocate %" PRId64 " numbers!", kCount);
    }

    for (auto i = INT64_C(0); i != kCount; i++) {
        numbers[i] = i;
    }

    auto sum = INT64_C(0);
    for (auto i = INT64_C(0); i != kCount; i++) {
        sum += numbers[i];
    }
    if (fprintf(stderr, "Sum is %" PRId64 ".\n", sum) < 0) {
        tomurcuk::Crashes::crash("Could not print the sum!");
    }

    memoryAllocator.deallocateArray(numbers, kCount);

    linearMemoryAllocator.destroy();
    return 0;
}
