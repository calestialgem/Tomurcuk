#pragma once

#include <assert.h>
#include <stdint.h>

namespace tomurcuk {
    template<typename Element>
    class ArrayReference {
    public:
        static auto of(Element *pointer) -> ArrayReference {
            ArrayReference arrayReference;
            arrayReference.mPointer = pointer;
            return arrayReference;
        }

        static auto null() -> ArrayReference {
            return of(nullptr);
        }

        auto isNull() -> bool {
            return mPointer == nullptr;
        }

        auto pointer() -> Element * {
            return mPointer;
        }

        auto get(int64_t index) -> Element * {
            assert(index >= 0);
            assert(mPointer != nullptr);

            return mPointer + index;
        }

    private:
        Element *mPointer;
    };
}
