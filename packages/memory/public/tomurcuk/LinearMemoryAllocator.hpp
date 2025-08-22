#pragma once

#include <stdint.h>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/VirtualBlock.hpp>

namespace tomurcuk {
    /**
     * Memory allocator that divides a single block of bytes with a cursor that
     * moves forward with each allocation.
     *
     * The backing memory is virtual. It is mapped as needed.
     */
    class LinearMemoryAllocator {
    public:
        /**
         * Creates a new allocator with a capacity.
         *
         * @see VirtualBlock::initialize
         *
         * @param[in] capacity The amount of virtual bytes that will be
         * allocated.
         * @return The actually allocated amount of virtual bytes on success. On
         * failure, `0`.
         */
        auto initialize(int64_t capacity) -> int64_t;

        /**
         * Deallocates all the virtual bytes.
         */
        auto destroy() -> void;

        /**
         * Provides it as a @ref MemoryAllocator.
         *
         * @return This allocator as a @ref MemoryAllocator.
         */
        auto getMemoryAllocator() -> MemoryAllocator;

        /**
         * Provides the amount of allocated bytes.
         *
         * @return The amount of allocated bytes.
         */
        auto getCursor() -> int64_t;

        /**
         * Deallocates all allocations after a previously queried cursor.
         *
         * @param[in] cursor The cursor that marks the first deallocated byte.
         */
        auto deallocateDownTo(int64_t cursor) -> void;

        /**
         * Deallocates all the allocations.
         */
        auto deallocateAll() -> void;

        /**
         * Unmaps all unused virtual bytes so that memory usage goes down.
         *
         * @return The actually unmapped amount of bytes on success. On failure,
         * `-1`.
         */
        auto releaseUnused() -> int64_t;

    private:
        /**
         * The implementation of @ref MemoryAllocator::mReallocate.
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
        static auto reallocateImplementation(void *state, void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool;

        /**
         * Changes the count of an array of bytes in the allocator.
         *
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
        auto reallocate(void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool;

        /**
         * Tests whether the given block's end corresponds to the current
         * position of the cursor.
         *
         * @return Whether the given block's end corresponds to the current
         * position of the cursor.
         */
        auto isLastAllocation(void *block, int64_t size) -> bool;

        /**
         * Allocates an amount bytes after the cursor.
         *
         * @param[in] size The amount of bytes that will be allocated.
         * @param[in] alignment The amount of bytes the address must be evenly
         * divisible to.
         * @return The address of the new allocation on success. On failure,
         * `nullptr`.
         */
        auto allocate(int64_t size, int64_t alignment) -> void *;

        /**
         * Backing storage.
         */
        VirtualBlock mVirtualBlock;

        /**
         * Amount of allocated bytes.
         */
        int64_t mCursor;
    };
}
