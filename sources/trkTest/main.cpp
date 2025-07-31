#include <stdio.h>
#include <trk/Crashes.hpp>
#include <trk/Demo.hpp>

auto main(int, char **) -> int {
    if (fprintf(stderr, "%s\n", trk::Demo::message()) < 0) {
        trk::Crashes::crash();
    }
    return 0;
}
