#include <stdlib.h>
#include <tomurcuk/Crashes.hpp>

auto trk::Crashes::crash() -> void {
    if (sCrashHandler != nullptr) {
        sCrashHandler();
    }
    abort();
}

auto trk::Crashes::setCrashHandler(void (*crashHandler)()) -> void {
    sCrashHandler = crashHandler;
}

void (*trk::Crashes::sCrashHandler)();
