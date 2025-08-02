#include <stdlib.h>
#include <tomurcuk/Crashes.hpp>

auto tomurcuk::Crashes::crash() -> void {
    if (sCrashHandler != nullptr) {
        sCrashHandler();
    }
    abort();
}

auto tomurcuk::Crashes::setCrashHandler(void (*crashHandler)()) -> void {
    sCrashHandler = crashHandler;
}

void (*tomurcuk::Crashes::sCrashHandler)();
