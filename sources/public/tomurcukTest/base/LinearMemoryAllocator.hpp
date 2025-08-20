#pragma once

#include <greatest.h>

namespace tomurcukTest {
    class LinearMemoryAllocator {
    public:
        static auto suite() -> void;

    private:
        static auto testAllocating() -> greatest_test_res;
        static auto testCursor() -> greatest_test_res;
        static auto testDeallocatingAll() -> greatest_test_res;
    };
}
