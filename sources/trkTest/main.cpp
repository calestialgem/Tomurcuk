#include <inttypes.h>
#include <stdio.h>
#include <trk/Crashes.hpp>
#include <trk/Demo.hpp>
#include <trk/PlatformError.hpp>
#include <trk/StandardError.hpp>

auto main(int, char **) -> int {
    if (fopen("some/invalid/path", "r") == nullptr) {
        static constexpr auto kCapacity = 1024;
        auto platformError = trk::PlatformError::getCurrent();
        auto standardError = trk::StandardError::getCurrent();
        char buffer[kCapacity];
        if (platformError.format(buffer, kCapacity) != 0) {
            (void)fprintf(stderr, "platform error 0x%08" PRIX32 ": %s\n", platformError.getCode(), buffer);
        }
        if (standardError.format(buffer, kCapacity) != 0) {
            (void)fprintf(stderr, "standard error 0x%08" PRIX32 ": %s\n", standardError.getCode(), buffer);
        }
        trk::Crashes::crash();
    }
    return 0;
}
