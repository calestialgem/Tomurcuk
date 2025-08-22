#pragma once

#include <greatest.h>

namespace tomurcuk {
    class ArrayOwnerTest {
    public:
        static auto suite() -> void;

    private:
        static auto testResizing() -> greatest_test_res;
    };
}
