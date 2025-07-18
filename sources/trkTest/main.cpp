#include <stdio.h>
#include <trk/Demo.hpp>

auto main(int, char **) -> int {
    (void)fprintf(stderr, "%s\n", trk::Demo::message());
    return 0;
}
