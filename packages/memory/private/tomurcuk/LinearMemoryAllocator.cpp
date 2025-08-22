#include <assert.h>
#include <stdint.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/LinearMemoryAllocator.hpp>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/VirtualBlock.hpp>

static_assert(sizeof(void *) == 8);

auto tomurcuk::LinearMemoryAllocator::initialize(int64_t capacity) -> int64_t {
    mCursor = 0;
    return mVirtualBlock.initialize(capacity);
}

auto tomurcuk::LinearMemoryAllocator::destroy() -> void {
    mVirtualBlock.destroy();
}

auto tomurcuk::LinearMemoryAllocator::getMemoryAllocator() -> MemoryAllocator {
    MemoryAllocator memoryAllocator;
    memoryAllocator.initialize(this, &reallocateImplementation);
    return memoryAllocator;
}

auto tomurcuk::LinearMemoryAllocator::getCursor() -> int64_t {
    return mCursor;
}

auto tomurcuk::LinearMemoryAllocator::deallocateDownTo(int64_t cursor) -> void {
    assert(cursor >= 0);
    assert(cursor <= mVirtualBlock.getLoad());

    mCursor = cursor;
}

auto tomurcuk::LinearMemoryAllocator::deallocateAll() -> void {
    mCursor = 0;
}

auto tomurcuk::LinearMemoryAllocator::releaseUnused() -> int64_t {
    return mVirtualBlock.release(mVirtualBlock.getLoad() - mCursor);
}

auto tomurcuk::LinearMemoryAllocator::reallocateImplementation(void *state, void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool {
    return ((LinearMemoryAllocator *)state)->reallocate(newBlock, oldBlock, oldSize, newSize, alignment);
}

auto tomurcuk::LinearMemoryAllocator::reallocate(void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool {
    assert(newBlock != nullptr);
    assert((oldBlock == nullptr) == (oldSize == 0));
    assert(oldSize >= 0);
    assert(newSize >= 0);
    assert(alignment > 0);

#if !defined(NDEBUG)
    if (oldBlock != nullptr) {
        auto validBegin = (uint64_t)mVirtualBlock.getAddress();
        auto validEnd = validBegin + (uint64_t)mCursor;
        auto claimBegin = (uint64_t)oldBlock;
        auto claimEnd = claimBegin + (uint64_t)oldSize;
        assert(claimBegin >= validBegin);
        assert(claimEnd <= validEnd);
    }
#endif

    // The operation is a deallocation.
    if (newSize == 0) {
        // Try reclaiming memory if this was the last allocation.
        if (isLastAllocation(oldBlock, oldSize)) {
            mCursor -= oldSize;
        }

        *newBlock = nullptr;
        return true;
    }

    // The operation is a reallocation.
    if (oldSize != 0) {
        // The operation wants to realign.
        if ((uint64_t)oldBlock % (uint64_t)alignment != 0) {
            *newBlock = allocate(newSize, alignment);
            if (*newBlock == nullptr) {
                return false;
            }
            if (oldSize < newSize) {
                Bytes::copyBlock(*newBlock, oldBlock, oldSize);
            } else {
                Bytes::copyBlock(*newBlock, oldBlock, newSize);
            }
            return true;
        }

        // Try resizing it if this was the last allocation.
        if (isLastAllocation(oldBlock, oldSize)) {
            // Reclaim if the operation shrinks the block.
            if (newSize <= oldSize) {
                mCursor -= oldSize - newSize;
                *newBlock = oldBlock;
                return true;
            }

            // Make the block grow in place.
            if (allocate(newSize - oldSize, 1) == nullptr) {
                return false;
            }
            *newBlock = oldBlock;
            return true;
        }

        // Try shrinking in place and leak the shrunk bytes.
        if (newSize <= oldSize) {
            *newBlock = oldBlock;
            return true;
        }

        // Leak the old block totally and just allocate a new one.
        *newBlock = allocate(newSize, alignment);
        if (*newBlock == nullptr) {
            return false;
        }
        Bytes::copyBlock(*newBlock, oldBlock, oldSize);
        return true;
    }

    // The operation is an allocation.
    *newBlock = allocate(newSize, alignment);
    return *newBlock != nullptr;
}

auto tomurcuk::LinearMemoryAllocator::isLastAllocation(void *block, int64_t size) -> bool {
    if (block == nullptr) {
        return false;
    }
    auto validEnd = (uint64_t)mVirtualBlock.getAddress() + (uint64_t)mCursor;
    auto claimEnd = (uint64_t)block + (uint64_t)size;
    return claimEnd == validEnd;
}

auto tomurcuk::LinearMemoryAllocator::allocate(int64_t size, int64_t alignment) -> void * {
    auto validEnd = (uint64_t)mVirtualBlock.getAddress() + (uint64_t)mCursor;
    auto reminder = validEnd % (uint64_t)alignment;
    auto padding = INT64_C(0);
    if (reminder != 0) {
        padding = alignment - (int64_t)reminder;
    }

    assert((uint64_t)padding <= UINT64_MAX - validEnd);
    assert(padding <= INT64_MAX - size);

    auto amount = size + padding;
    auto space = mVirtualBlock.getLoad() - mCursor;
    if (space < amount) {
        auto requiredSpace = amount - space;
        if (mVirtualBlock.reserve(requiredSpace) < requiredSpace) {
            return nullptr;
        }
    }
    auto block = (void *)((char *)mVirtualBlock.getAddress() + mCursor + padding);
    mCursor += amount;
    return block;
}
