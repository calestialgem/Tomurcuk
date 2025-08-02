#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tomurcuk/Crashes.hpp>
#include <tomurcuk/PlatformError.hpp>
#include <tomurcuk/StandardError.hpp>

auto tomurcuk::Crashes::crash(char *format, ...) -> void {
    auto arguments = va_list{};
    va_start(arguments, format);
    crashWith(format, arguments);
    va_end(arguments);
}

auto tomurcuk::Crashes::crashWith(char *format, va_list arguments) -> void {
    auto platformError = PlatformError::getCurrent();
    auto standardError = StandardError::getCurrent();

    static constexpr auto kCapacity = 1024;
    char buffer[kCapacity];

    // Report the platform error.
    {
        auto load = platformError.format(buffer, kCapacity);
        if (load == 0) {
            auto messageError = PlatformError::getCurrent();
            (void)fprintf(stderr, "platform error was 0x%08" PRIX32 ", but could not get its message because of 0x%08" PRIX32 "\n", platformError.getCode(), messageError.getCode());
        } else {
            load = removeTrailingNewLines(buffer, load);
            (void)fprintf(stderr, "platform error 0x%08" PRIX32 ": %s\n", platformError.getCode(), buffer);
        }
    }

    // Report the standard error.
    {
        auto load = standardError.format(buffer, kCapacity);
        if (load == 0) {
            auto messageError = StandardError::getCurrent();
            (void)fprintf(stderr, "standard error was 0x%08" PRIX32 ", but could not get its message because of 0x%08" PRIX32 "\n", standardError.getCode(), messageError.getCode());
        } else {
            load = removeTrailingNewLines(buffer, load);
            (void)fprintf(stderr, "standard error 0x%08" PRIX32 ": %s\n", standardError.getCode(), buffer);
        }
    }

    // Format the crash message.
    {
        auto formatStatus = vsnprintf(buffer, kCapacity, format, arguments);
        if (formatStatus < 0) {
            auto formatError = StandardError::getCurrent();
            (void)fprintf(stderr, "Could not format the crash message\n");

            // Report the format error.
            auto load = formatError.format(buffer, kCapacity);
            if (load == 0) {
                auto messageError = StandardError::getCurrent();
                (void)fprintf(stderr, "format error was 0x%08" PRIX32 ", but could not get its message because of 0x%08" PRIX32 "\n", formatError.getCode(), messageError.getCode());
            } else {
                load = removeTrailingNewLines(buffer, load);
                (void)fprintf(stderr, "format error 0x%08" PRIX32 ": %s\n", formatError.getCode(), buffer);
            }
        } else {
            // Report the crash message.
            auto load = (uint64_t)formatStatus + 1;
            load = removeTrailingNewLines(buffer, load);
            (void)fprintf(stderr, "crash: %s\n", buffer);
        }
    }

    if (sCrashHandler != nullptr) {
        sCrashHandler();
    }
    abort();
}

auto tomurcuk::Crashes::setCrashHandler(void (*crashHandler)()) -> void {
    sCrashHandler = crashHandler;
}

void (*tomurcuk::Crashes::sCrashHandler)();

auto tomurcuk::Crashes::removeTrailingNewLines(char *buffer, uint64_t load) -> uint64_t {
    auto wasTerminated = load > 0 && buffer[load - 1] == 0;
    auto length = load - (uint64_t)wasTerminated;

    // Count the new lines.
    auto trailingBytes = UINT64_C(0);
    auto lastByte = buffer[length - trailingBytes - 1];
    if (lastByte == '\n') {
        // Handle the last byte specially, so that, the loop does not need to
        // check whether there is a previous byte.
        auto previousByte = lastByte;
        for (trailingBytes = 1; trailingBytes != length; trailingBytes++) {
            auto currentByte = buffer[length - trailingBytes - 1];
            if (currentByte != '\n' && (currentByte != '\r' || previousByte != '\n')) {
                break;
            }
            previousByte = currentByte;
        }
    }

    // Remove the new lines.
    load -= trailingBytes;
    if (wasTerminated) {
        buffer[load - 1] = 0;
    }
    return load;
}
