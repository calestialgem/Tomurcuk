#pragma once

#include <stdint.h>

namespace tomurcuk {
    /*
     * Abstracts over memory allocation schemes.
     */
    class MemoryAllocator {
    public:
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
            void *newBlock;
            auto status = allocateBlock(&newBlock, newCount, sizeof(Element), alignof(Element));
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
            void *newBlock;
            auto status = reallocateBlock(&newBlock, oldArray, oldCount, newCount, sizeof(Element), alignof(Element));
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
            deallocateBlock(oldArray, oldCount, sizeof(Element), alignof(Element));
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
            auto status = allocateBlock(&newBlock, 1, sizeof(Object), alignof(Object));
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
            deallocateBlock(oldObject, 1, sizeof(Object), alignof(Object));
        }

    private:
        /**
         * Allocates an array of bytes from the allocator.
         *
         * @param[out] newBlock The pointer to the newly allocated block.
         * @param[in] newCount The amount of elements that is desired to be in
         * the resulting block.
         * @param[in] size The amount of bytes in an element.
         * @return Whether the request succeeded.
         */
        auto allocateBlock(void **newBlock, int64_t newCount, int64_t size, int64_t alignment) -> bool;

        /**
         * Changes the count of an array of bytes in the allocator.
         *
         * @param[out] newBlock The pointer to the reallocated block.
         * @param[in] oldBlock The pointer to the block that will be reallocated.
         * Must be `nullptr` if `oldCount` is `0`.
         * @param[in] oldCount The amount of elements in `oldBlock`.
         * @param[in] newCount The amount of elements that is desired to be in the
         * resulting block.
         * @param[in] size The amount of bytes in an element.
         * @param[in] alignment The amount of bytes the elements' address must be
         * evenly divisible to.
         * @return Whether the request succeeded.
         */
        auto reallocateBlock(void **newBlock, void *oldBlock, int64_t oldCount, int64_t newCount, int64_t size, int64_t alignment) -> bool;

        /**
         * Relinquishes an array of bytes back to the allocator.
         *
         * @param[in] oldBlock The pointer to the block that will be deallocated.
         * Must be `nullptr` if `oldCount` is `0`.
         * @param[in] oldCount The amount of elements in `oldBlock`.
         * @param[in] size The amount of bytes in an element.
         * @param[in] alignment The amount of bytes the elements' address must be
         * evenly divisible to.
         */
        auto deallocateBlock(void *oldBlock, int64_t oldCount, int64_t size, int64_t alignment) -> void;

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
         * Must be `nullptr` if `oldCount` is `0`.
         * @param[in] oldCount The amount of elements in `oldBlock`.
         * @param[in] newCount The amount of elements that is desired to be in the
         * resulting block.
         * @param[in] size The amount of bytes in an element.
         * @param[in] alignment The amount of bytes the elements' address must be
         * evenly divisible to.
         * @return Whether the request succeeded.
         */
        bool (*mReallocate)(void *state, void **newBlock, void *oldBlock, int64_t oldCount, int64_t newCount, int64_t size, int64_t alignment);
    };
}
