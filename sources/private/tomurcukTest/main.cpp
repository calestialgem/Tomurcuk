#include <greatest.h>
#include <tomurcukTest/base/LinearMemoryAllocator.hpp>

GREATEST_MAIN_DEFS(); // NOLINT

auto main(int argc, char **argv) -> int {
    GREATEST_MAIN_BEGIN();
    GREATEST_RUN_SUITE(tomurcukTest::LinearMemoryAllocator::suite);
    GREATEST_MAIN_END();
}
