#pragma once

#include <greatest.h>

namespace tomurcuk {
    class LinearMemoryAllocatorTest {
    public:
        static auto suite() -> void;

    private:
        static auto testAllocating() -> greatest_test_res;
        static auto testCursor() -> greatest_test_res;
        static auto testDeallocatingAll() -> greatest_test_res;
    };
}
