#include <greatest.h>
#include <tomurcuk/ArrayOwnerTest.hpp>
#include <tomurcuk/LinearMemoryAllocatorTest.hpp>

GREATEST_MAIN_DEFS(); // NOLINT

auto main(int argc, char **argv) -> int {
    GREATEST_MAIN_BEGIN();
    GREATEST_RUN_SUITE(tomurcuk::ArrayOwnerTest::suite);
    GREATEST_RUN_SUITE(tomurcuk::LinearMemoryAllocatorTest::suite);
    GREATEST_MAIN_END();
}
