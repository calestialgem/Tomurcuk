#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/ArrayListView.hpp>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/MemoryAllocator.hpp>

namespace tomurcuk {
    /**
     * Contiguous block of memory that holds elements.
     *
     * @tparam Element The type of the elements.
     *
     * @warning The backing memory might not be allocated. That case should act
     * as if there are no elements in the list.
     *
     * The list is divided into two parts: initialized and uninitialized. The
     * initialized part holds the elements. It is at the beginning of the block
     * and grows into the uninitialized part as more elements are added.
     *
     * To append to the list:
     *   1- Acquire required slots via @ref reserve.
     *   2- Write to them via @ref getEnd and @ref getUninitializedCount.
     *   3- Mark the newly written elements as initialized via @ref acknowledge.
     */
    template<typename Element>
    class ArrayList {
    public:
        /**
         * Creates a new list that is empty.
         */
        auto initialize() -> void {
            mArray = nullptr;
            mCapacity = 0;
            mCount = 0;
        }

        /**
         * Deallocates the backing memory.
         *
         * @param[in,out] memoryAllocator The allocator that did provide the
         * memory.
         */
        auto destroy(MemoryAllocator memoryAllocator) -> void {
            memoryAllocator.deallocateArray(mArray, mCapacity);
        }

        /**
         * Provides a view of the list.
         *
         * @warning The list must not be modified while the view is used.
         *
         * @return A view that refers to this list.
         */
        auto getView() -> ArrayListView<Element> {
            ArrayListView<Element> view;
            view.initialize(mArray, mCount);
            return view;
        }

        /**
         * Provides the backing array.
         *
         * @return The pointer to the backing array if it exists. Otherwise,
         * `nullptr`.
         */
        auto getArray() -> Element * {
            return mArray;
        }

        /**
         * Provides the uninitialized slot that will hold the next added
         * element.
         *
         * @return The pointer to the first uninitialized element if there is
         * one. Otherwise, the pointer to the slot after the last initialized
         * element if there is one. Otherwise, `nullptr`.
         */
        auto getEnd() -> Element * {
            return mArray + mCount;
        }

        /**
         * Provides the amount of initialized elements.
         *
         * @return The amount of initialized elements in the backing memory of
         * the list.
         */
        auto getCount() -> int64_t {
            return mCount;
        }

        /**
         * Provides the amount of initialized bytes.
         *
         * @return The total amount of bytes that constitute the initialized
         * elements.
         */
        auto getSize() -> int64_t {
            assert(getCount() <= INT64_MAX / (int64_t)sizeof(Element));

            return getCount() * (int64_t)sizeof(Element);
        }

        /**
         * Provides the amount of allocated elements.
         *
         * @return The amount of initialized elements in the backing memory of
         * the list.
         */
        auto getAllocatedCount() -> int64_t {
            return mCapacity;
        }

        /**
         * Provides the amount of allocated bytes.
         *
         * @return The total amount of bytes that constitute the allocated
         * elements.
         */
        auto getAllocatedSize() -> int64_t {
            assert(getAllocatedCount() <= INT64_MAX / (int64_t)sizeof(Element));

            return getAllocatedCount() * (int64_t)sizeof(Element);
        }

        /**
         * Provides the amount of uninitialized elements.
         *
         * @return The amount of uninitialized elements in the backing memory of
         * the list.
         */
        auto getUninitializedCount() -> int64_t {
            return mCapacity - mCount;
        }

        /**
         * Provides the amount of uninitialized bytes.
         *
         * @return The total amount of bytes that constitute the uninitialized
         * elements.
         */
        auto getUninitializedSize() -> int64_t {
            assert(getUninitializedCount() <= INT64_MAX / (int64_t)sizeof(Element));

            return getUninitializedCount() * (int64_t)sizeof(Element);
        }

        /**
         * Tests whether there are no initialized elements.
         *
         * @return Whether there are no initialized elements in the list.
         */
        auto isEmpty() -> bool {
            return mCount == 0;
        }

        /**
         * Provides the pointer to the first element.
         *
         * @return The pointer to the first initialized element if there is one.
         * Otherwise, the pointer to the first uninitialized element if there is
         * one. Otherwise, `nullptr`.
         */
        auto getFirst() -> Element * {
            return mArray;
        }

        /**
         * Provides the pointer to the last element.
         *
         * @return The pointer to the last initialized element if there is one.
         * Otherwise, the pointer to the first uninitialized element if there is
         * one. Otherwise, `nullptr`.
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

        /**
         * Removes the first element.
         *
         * @return The element that was previously the first one.
         */
        auto removeFirst() -> Element {
            return remove(0);
        }

        /**
         * Removes the last element.
         *
         * @return The element that was previously the last one.
         */
        auto removeLast() -> Element {
            return remove(mCount - 1);
        }

        /**
         * Removes the element at an index.
         *
         * @param[in] index The amount of elements before the accessed element.
         * @return The element that was previously at the given index.
         */
        auto remove(int64_t index) -> Element {
            auto element = get(index);
            Bytes::copyAliasingArray(mArray + index, mArray + index + 1, mCount - index - 1);
            mCount--;
            return element;
        }

        /**
         * Removes the element at an index without preserving the order of the
         * elements.
         *
         * This gets faster than @ref remove as the list gets bigger and the
         * removed element is close to the beginning.
         *
         * @param[in] index The amount of elements before the accessed element.
         * @return The element that was previously at the given index.
         */
        auto removeUnordered(int64_t index) -> Element {
            auto element = get(index);
            mCount--;
            if (!isEmpty()) {
                mArray[index] = mArray[mCount];
            }
            return element;
        }

        /**
         * Removes a portion of the elements from the list.
         *
         * @warning This is faster than calling @ref remove one by one.
         *
         * @param[in] beginIndex The amount of elements from the beginning that
         * will not be removed.
         * @param[in] endIndex The total amount of elements from the beginning
         * that will be considered to be removed.
         */
        auto removePortion(int64_t beginIndex, int64_t endIndex) -> void {
            assert(beginIndex >= 0);
            assert(beginIndex <= endIndex);
            assert(endIndex <= mCount);

            Bytes::copyAliasingArray(mArray + beginIndex, mArray + endIndex, mCount - endIndex);
            mCount -= endIndex - beginIndex;
        }

        /**
         * Removes all the elements from the list.
         *
         * @warning This is faster than calling @ref remove one by one.
         */
        auto removeAll() -> void {
            removePortion(0, mCount);
        }

        /**
         * Appends another list to the end of this one.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] view The added elements.
         * @return Whether the operation succeeded.
         */
        auto addAll(MemoryAllocator memoryAllocator, ArrayListView<Element> view) -> bool {
            if (!reserve(memoryAllocator, view.getCount())) {
                return false;
            }
            Bytes::copyArray(getEnd(), view.getArray(), view.getCount());
            acknowledge(view.getCount());
            return true;
        }

        /**
         * Appends an element to the end of the list.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] element The added element.
         * @return Whether the operation succeeded.
         */
        auto add(MemoryAllocator memoryAllocator, Element element) -> bool {
            if (!reserve(memoryAllocator, 1)) {
                return false;
            }
            getEnd()[0] = element;
            acknowledge(1);
            return true;
        }

        /**
         * Appends another list to this one at an index.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] index The amount of elements before the first newly
         * inserted element.
         * @param[in] view The inserted elements.
         * @return Whether the operation succeeded.
         */
        auto insertAll(MemoryAllocator memoryAllocator, int64_t index, ArrayListView<Element> view) -> bool {
            assert(index >= 0);
            assert(index < mCount);

            if (!reserve(memoryAllocator, view.getCount())) {
                return false;
            }
            Bytes::copyAliasingArray(mArray + index + view.getCount(), mArray + index, mCount - index);
            Bytes::copyArray(mArray + index, view.getArray(), view.getCount());
            acknowledge(view.getCount());
            return true;
        }

        /**
         * Appends an element to the list at an index.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] index The amount of elements before the newly inserted
         * element.
         * @param[in] element The inserted element.
         * @return Whether the operation succeeded.
         */
        auto insert(MemoryAllocator memoryAllocator, int64_t index, Element element) -> bool {
            assert(index >= 0);
            assert(index < mCount);

            if (!reserve(memoryAllocator, 1)) {
                return false;
            }
            Bytes::copyAliasingArray(mArray + index + 1, mArray + index, mCount - index);
            mArray[index] = element;
            acknowledge(1);
            return true;
        }

        /**
         * Grows the amount of uninitialized elements in preparation for an
         * append-operation.
         *
         * @param[in,out] memoryAllocator The allocator that will/did provide
         * the memory.
         * @param[in] amount The least amount of uninitialized elements that
         * must exists in the list.
         * @return Whether the request succeeded.
         */
        auto reserve(MemoryAllocator memoryAllocator, int64_t amount) -> bool {
            auto newCapacity = Bytes::growCapacity(mCapacity, mCount, amount);
            if (newCapacity == mCapacity) {
                return true;
            }

            Element *newArray;
            if (!memoryAllocator.reallocateArray(&newArray, mArray, mCapacity, newCapacity)) {
                return false;
            }

            mArray = newArray;
            mCapacity = newCapacity;
            return true;
        }

        /**
         * Marks some amount of leading uninitialized elements as initialized.
         *
         * @param[in] amount The amount of uninitialized elements that will be
         * marked as initialized.
         */
        auto acknowledge(int64_t amount) -> void {
            assert(amount >= 0);
            assert(amount <= mCapacity - mCount);

            mCount += amount;
        }

    private:
        /**
         * Pointer to the backing memory.
         *
         * @warning `nullptr` if there are no allocated elements.
         */
        Element *mArray;

        /**
         * The amount of allocated elements.
         */
        int64_t mCapacity;

        /**
         * The amount of initialized elements.
         *
         * This is always less than `mCapacity`.
         */
        int64_t mCount;
    };
}
