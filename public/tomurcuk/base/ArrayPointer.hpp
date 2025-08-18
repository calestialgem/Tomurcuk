#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/base/MemoryAllocator.hpp>

namespace tomurcuk {
    /**
     * A wrapper around a raw pointer to an array, which might be `nullptr`.
     *
     * This might own the array, or it might be a reference to an array.
     *
     * @warning The pointer might be `nullptr`. That case must work as if it was
     * an array of length `0`.
     *
     * @tparam Element The array's members' type.
     */
    template<typename Element>
    class ArrayPointer {
    public:
        /**
         * Creates a new @ref ArrayPointer.
         *
         * @return An @ref ArrayPointer who points to `nullptr`.
         */
        static auto create() -> ArrayPointer<Element> {
            ArrayPointer<Element> arrayPointer;
            arrayPointer.mBegin = nullptr;
            return arrayPointer;
        }

        /**
         * Deallocates the pointed array.
         *
         * @param[in,out] memoryAllocator The allocator that did provide the
         * memory.
         * @param[in] length The amount of elements in the array.
         */
        auto destroy(MemoryAllocator memoryAllocator, int64_t length) -> void {
            assert(length >= 0);

            memoryAllocator.deallocateArray(mBegin, length);
        }

        /**
         * Reallocates the pointed array.
         *
         * @warning If the array grows, the newly allocated elements will be
         * uninitialized.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] oldLength The current amount of elements in the array.
         * @param[in] newLength The desired amount of elements in the array.
         */
        auto resize(MemoryAllocator memoryAllocator, int64_t oldLength, int64_t newLength) -> bool {
            assert(oldLength >= 0);
            assert(newLength >= 0);

            Element *newBegin;
            if (!memoryAllocator.reallocateArray(&newBegin, mBegin, oldLength, newLength)) {
                return false;
            }
            mBegin = newBegin;
            return true;
        }

        /**
         * Provides a raw pointer to the pointed array.
         *
         * @return A pointer to the first element of the array if it exists.
         * Otherwise, pointer to the address after the last element if the
         * pointed array is not `nullptr`. Otherwise, `nullptr`.
         */
        auto getBegin() -> Element * {
            return mBegin;
        }

        /**
         * Provides a pointer to the element at an index.
         *
         * @param[in] index The amount of elements that come before the accessed
         * one.
         * @return A pointer to the element at the given index.
         */
        auto get(int64_t index) -> Element * {
            assert(mBegin != nullptr);
            assert(index >= 0);

            return mBegin + index;
        }

    private:
        /**
         * The pointed array.
         *
         * @warning This might be `nullptr`.
         */
        Element *mBegin;
    };
}
