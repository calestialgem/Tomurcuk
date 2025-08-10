#pragma once

#include <assert.h>
#include <stdint.h>

namespace tomurcuk {
    /*
     * Abstracts over memory allocation schemes.
     */
    class MemoryAllocator {
    public:
        /**
         * Creates a new allocator abstraction.
         *
         * @param[in,out] state The pointer to the state of the allocator.
         * @param[in] reallocate The implementation of @ref mReallocate.
         */
        auto initialize(void *state, bool (*reallocate)(void *state, void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment)) -> void;

        /**
         * Allocates elements from the allocator.
         *
         * @tparam Element The type of the elements.
         * @param[out] newArray The pointer to the newly allocated array.
         * @param[in] newCount The amount of elements that is desired to be in
         * the resulting array.
         * @return Whether the request succeeded.
         */
        template<typename Element>
        auto allocateArray(Element **newArray, int64_t newCount) -> bool {
            assert(newCount <= INT64_MAX / (int64_t)sizeof(Element));

            void *newBlock;
            auto status = allocateBlock(&newBlock, newCount * (int64_t)sizeof(Element), alignof(Element));
            if (status) {
                *newArray = (Element *)newBlock;
            }
            return status;
        }

        /**
         * Changes the amount of elements in the allocator.
         *
         * @tparam Element The type of the elements.
         * @param[out] newArray The pointer to the reallocated array.
         * @param[in] oldArray The pointer to the array that will be
         * reallocated. Must be `nullptr` if `oldCount` is `0`.
         * @param[in] oldCount The amount of elements in `oldArray`.
         * @param[in] newCount The amount of elements that is desired to be in
         * the resulting array.
         * @return Whether the request succeeded.
         */
        template<typename Element>
        auto reallocateArray(Element **newArray, Element *oldArray, int64_t oldCount, int64_t newCount) -> bool {
            assert(oldCount <= INT64_MAX / (int64_t)sizeof(Element));
            assert(newCount <= INT64_MAX / (int64_t)sizeof(Element));

            void *newBlock;
            auto status = reallocateBlock(&newBlock, oldArray, oldCount * (int64_t)sizeof(Element), newCount * sizeof(Element), alignof(Element));
            if (status) {
                *newArray = (Element *)newBlock;
            }
            return status;
        }

        /**
         * Relinquishes elements back to the allocator.
         *
         * @tparam Element The type of the elements.
         * @param[in] oldArray The pointer to the array that will be
         * deallocated.
         * Must be `nullptr` if `oldCount` is `0`.
         * @param[in] oldCount The amount of elements in `oldArray`.
         * evenly divisible to.
         */
        template<typename Element>
        auto deallocateArray(Element *oldArray, int64_t oldCount) -> void {
            assert(oldCount <= INT64_MAX / (int64_t)sizeof(Element));

            deallocateBlock(oldArray, oldCount * (int64_t)sizeof(Element), alignof(Element));
        }

        /**
         * Allocates an object from the allocator.
         *
         * @tparam Object The type of the object.
         * @param[out] newObject The pointer to the newly allocated object.
         * @return Whether the request succeeded.
         */
        template<typename Object>
        auto allocateObject(Object **newObject) -> bool {
            void *newBlock;
            auto status = allocateBlock(&newBlock, sizeof(Object), alignof(Object));
            if (status) {
                *newObject = (Object *)newBlock;
            }
            return status;
        }

        /**
         * Relinquishes an object back to the allocator.
         *
         * @tparam Object The type of the object.
         * @param[in] oldObject The pointer to the object that will be
         * deallocated.
         */
        template<typename Object>
        auto deallocateObject(Object *oldObject) -> void {
            deallocateBlock(oldObject, sizeof(Object), alignof(Object));
        }

        /**
         * Allocates an array of bytes from the allocator.
         *
         * @param[out] newBlock The pointer to the newly allocated block.
         * @param[in] newSize The amount of bytes that is desired to be in
         * the resulting block.
         * @return Whether the request succeeded.
         */
        auto allocateBlock(void **newBlock, int64_t newSize, int64_t alignment) -> bool;

        /**
         * Changes the size of an array of bytes in the allocator.
         *
         * @param[out] newBlock The pointer to the reallocated block.
         * @param[in] oldBlock The pointer to the block that will be reallocated.
         * Must be `nullptr` if `oldSize` is `0`.
         * @param[in] oldSize The amount of bytes in `oldBlock`.
         * @param[in] newSize The amount of bytes that is desired to be in the
         * resulting block.
         * @param[in] alignment The amount of bytes the address must be evenly
         * divisible to.
         * @return Whether the request succeeded.
         */
        auto reallocateBlock(void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool;

        /**
         * Relinquishes an array of bytes back to the allocator.
         *
         * @param[in] oldBlock The pointer to the block that will be deallocated.
         * Must be `nullptr` if `oldSize` is `0`.
         * @param[in] oldSize The amount of bytes in `oldBlock`.
         * @param[in] alignment The amount of bytes the address must be evenly
         * divisible to.
         */
        auto deallocateBlock(void *oldBlock, int64_t oldSize, int64_t alignment) -> void;

    private:
        /**
         * Data of the implementation.
         */
        void *mState;

        /**
         * The implementation of the allocator.
         *
         * @param[in,out] state The pointer to the state of the allocator.
         * @param[out] newBlock The pointer to the newly allocated block.
         * @param[in] oldBlock The pointer to the block that will be reallocated.
         * Must be `nullptr` if `oldSize` is `0`.
         * @param[in] oldSize The amount of bytes in `oldBlock`.
         * @param[in] newSize The amount of bytes that is desired to be in the
         * resulting block.
         * @param[in] alignment The amount of bytes the address must be evenly
         * divisible to.
         * @return Whether the request succeeded.
         */
        bool (*mReallocate)(void *state, void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment);
    };
}
