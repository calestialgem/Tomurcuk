#include <inttypes.h>
#include <stdio.h>
#include <tomurcuk/Crashes.hpp>
#include <tomurcuk/Demo.hpp>
#include <tomurcuk/PlatformError.hpp>
#include <tomurcuk/StandardError.hpp>

auto main(int, char **) -> int {
    static constexpr char kPath[] = "some/invalid/path";
    if (fopen(kPath, "r") == nullptr) {
        tomurcuk::Crashes::crash("Could not open the file at path `%s`", kPath);
    }
    return 0;
}
