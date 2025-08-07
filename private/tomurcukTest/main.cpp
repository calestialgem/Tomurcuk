#include <inttypes.h>
#include <stdio.h>
#include <tomurcuk/status/Crashes.hpp>

auto main(int, char **) -> int {
    static constexpr auto kPath = "some/invalid/path";
    if (fopen(kPath, "r") == nullptr) {
        tomurcuk::Crashes::crash("Could not open the file at path `%s`", kPath);
    }
    return 0;
}
