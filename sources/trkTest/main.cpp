#include <stdio.h>
#include <trk/Crashes.hpp>
#include <trk/Demo.hpp>
#include <trk/StandardError.hpp>

auto main(int, char **) -> int {
    if (fopen("some/invalid/path", "r") == nullptr) {
        static constexpr auto kCapacity = 1024;
        auto error = trk::StandardError::getCurrent();
        char buffer[kCapacity];
        if (error.format(buffer, kCapacity) != 0) {
            (void)fprintf(stderr, "standard error: %s\n", buffer);
        }
        trk::Crashes::crash();
    }
    return 0;
}
