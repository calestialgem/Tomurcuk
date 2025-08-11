#pragma once

#include <assert.h>
#include <stdint.h>

namespace tomurcuk {
    /**
     * Reference to a contiguous block of memory that holds elements.
     *
     * @tparam Element The type of the elements.
     *
     * @warning Reference might not be a view of any array. That case should act
     * as if an empty array was viewed.
     */
    template<typename Element>
    class ArrayListView {
    public:
        /**
         * Creates a reference to an array.
         *
         * @param[in] array The referred array.
         * @param[in] count The amount of referred elements.
         */
        auto initialize(Element *array, int64_t count) -> void {
            assert((array == nullptr) == (count == 0));
            assert(count >= 0);

            mArray = array;
            mCount = count;
        }

        /**
         * Creates an empty view.
         */
        auto initializeEmpty() -> void {
            mArray = nullptr;
            mCount = 0;
        }

        /**
         * Provides the referred array.
         *
         * @return The pointer to the referred array if it exists. Otherwise,
         * `nullptr`.
         */
        auto getArray() -> Element * {
            return mArray;
        }

        /**
         * Provides the amount of referred elements.
         *
         * @return The amount of referred elements.
         */
        auto getCount() -> int64_t {
            return mCount;
        }

        /**
         * Provides the amount of referred bytes.
         *
         * @return The total amount of bytes that constitute the referred
         * elements.
         */
        auto getSize() -> int64_t {
            assert(mCount <= INT64_MAX / (int64_t)sizeof(mCount));

            return mCount * (int64_t)sizeof(mCount);
        }

        /**
         * Tests whether there are no elements.
         *
         * @return Whether there are no elements in the view.
         */
        auto isEmpty() -> bool {
            return mCount == 0;
        }

        /**
         * Provides the pointer to the first element.
         *
         * @return The pointer to the first element if there is one. Otherwise,
         * the pointer to the referred array.
         */
        auto getFirst() -> Element * {
            return mArray;
        }

        /**
         * Provides the pointer to the last element.
         *
         * @return The pointer to the last element if there is one. Otherwise,
         * the pointer to the referred array.
         */
        auto getLast() -> Element * {
            return mArray + mCount - (mCount != 0);
        }

        /**
         * Provides the pointer to the element at an index.
         *
         * @param[in] index The amount of elements before the accessed element.
         * @return The pointer to the element at the given index.
         */
        auto get(int64_t index) -> Element * {
            assert(index >= 0);
            assert(index < mCount);

            return mArray[index];
        }

    private:
        /**
         * Pointer to the referred array.
         *
         * @warning Might be `nullptr` if there are no elements.
         */
        Element *mArray;

        /**
         * The amount of referred elements.
         */
        int64_t mCount;
    };
}
