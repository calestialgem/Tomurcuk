/*
 * `ArrayPointer` annotates a pointer as an indirect array. It provides helper
 * functions that calculate array size for the user.
 *
 * `ArrayPointer` might be wrapping a `nullptr`. That state should work like an
 * empty array.
 *
 * `ArrayPointer` might be the owner of the pointed array. If that is the case,
 * the lifetime of the pointed array should be managed through its functions.
 *
 * `ArrayPointer` does not store the length of the array. `Array` or `ArrayView`
 * should be used if that is desired.
 */

#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/base/MemoryAllocator.hpp>

namespace tomurcuk {
    template<typename Element>
    class ArrayPointer {
    public:
        static auto of(Element *pointer) -> ArrayPointer<Element> {
            ArrayPointer<Element> arrayPointer;
            arrayPointer.mPointer = pointer;
            return arrayPointer;
        }

        static auto null() -> ArrayPointer<Element> {
            return of(nullptr);
        }

        auto allocate(MemoryAllocator memoryAllocator, int64_t newLength) -> bool {
            assert(newLength >= 0);
            assert(newLength <= INT64_MAX / (int64_t)sizeof(Element));

            void *newPointer;
            if (!memoryAllocator.allocateBlock(&newPointer, mPointer, newLength * (int64_t)sizeof(Element), alignof(Element))) {
                return false;
            }

            mPointer = newPointer;
            return true;
        }

        auto reallocate(MemoryAllocator memoryAllocator, int64_t oldLength, int64_t newLength) -> bool {
            assert(oldLength >= 0);
            assert(oldLength <= INT64_MAX / (int64_t)sizeof(Element));
            assert(newLength >= 0);
            assert(newLength <= INT64_MAX / (int64_t)sizeof(Element));

            void *newPointer;
            if (!memoryAllocator.reallocateBlock(&newPointer, mPointer, oldLength * (int64_t)sizeof(Element), newLength * (int64_t)sizeof(Element), alignof(Element))) {
                return false;
            }

            mPointer = newPointer;
            return true;
        }

        auto deallocate(MemoryAllocator memoryAllocator, int64_t oldLength) -> void {
            assert(oldLength >= 0);
            assert(oldLength <= INT64_MAX / (int64_t)sizeof(Element));

            memoryAllocator.deallocateBlock(mPointer, oldLength * (int64_t)sizeof(Element), alignof(Element));
        }

        auto pointer() -> Element * {
            return pointer;
        }

    private:
        Element *mPointer;
    };
}
