#include <assert.h>
#include <stdint.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/LinearMemoryAllocator.hpp>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Status.hpp>
#include <tomurcuk/VirtualBlock.hpp>

static_assert(sizeof(void *) == 8);

auto tomurcuk::LinearMemoryAllocator::create(int64_t capacity) -> Result<LinearMemoryAllocator> {
    auto virtualBlock = VirtualBlock::create(capacity);
    if (virtualBlock.isFailure()) {
        return Result<LinearMemoryAllocator>::failure();
    }

    LinearMemoryAllocator linearMemoryAllocator;
    linearMemoryAllocator.mVirtualBlock = *virtualBlock.value();
    linearMemoryAllocator.mCursor = 0;
    return Result<LinearMemoryAllocator>::success(linearMemoryAllocator);
}

auto tomurcuk::LinearMemoryAllocator::destroy() -> void {
    mVirtualBlock.destroy();
}

auto tomurcuk::LinearMemoryAllocator::memoryAllocator() -> MemoryAllocator {
    return MemoryAllocator::create(this, &reallocateImplementation);
}

auto tomurcuk::LinearMemoryAllocator::cursor() -> int64_t {
    return mCursor;
}

auto tomurcuk::LinearMemoryAllocator::deallocateDownTo(int64_t cursor) -> void {
    assert(cursor >= 0);
    assert(cursor <= mVirtualBlock.load());

    mCursor = cursor;
}

auto tomurcuk::LinearMemoryAllocator::deallocateAll() -> void {
    mCursor = 0;
}

auto tomurcuk::LinearMemoryAllocator::releaseUnused() -> Status {
    return mVirtualBlock.release(mVirtualBlock.load() - mCursor);
}

auto tomurcuk::LinearMemoryAllocator::reallocateImplementation(void *state, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *> {
    return ((LinearMemoryAllocator *)state)->reallocate(oldBlock, oldSize, newSize, alignment);
}

auto tomurcuk::LinearMemoryAllocator::reallocate(void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *> {
    assert((oldBlock == nullptr) == (oldSize == 0));
    assert(oldSize >= 0);
    assert(newSize >= 0);
    assert(alignment > 0);

#if !defined(NDEBUG)
    if (oldBlock != nullptr) {
        auto validBegin = (uint64_t)mVirtualBlock.address();
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

        return Result<void *>::success(nullptr);
    }

    // The operation is a reallocation.
    if (oldSize != 0) {
        // The operation wants to realign.
        if ((uint64_t)oldBlock % (uint64_t)alignment != 0) {
            auto newBlock = allocate(newSize, alignment);
            if (newBlock.isSuccess()) {
                if (oldSize < newSize) {
                    Bytes::copyBlock(*newBlock.value(), oldBlock, oldSize);
                } else {
                    Bytes::copyBlock(*newBlock.value(), oldBlock, newSize);
                }
            }
            return newBlock;
        }

        // Try resizing it if this was the last allocation.
        if (isLastAllocation(oldBlock, oldSize)) {
            // Reclaim if the operation shrinks the block.
            if (newSize <= oldSize) {
                mCursor -= oldSize - newSize;
                return Result<void *>::success(oldBlock);
            }

            // Make the block grow in place.
            if (allocate(newSize - oldSize, 1).isFailure()) {
                return Result<void *>::failure();
            }
            return Result<void *>::success(oldBlock);
        }

        // Try shrinking in place and leak the shrunk bytes.
        if (newSize <= oldSize) {
            return Result<void *>::success(oldBlock);
        }

        // Leak the old block totally and just allocate a new one.
        auto newBlock = allocate(newSize, alignment);
        if (newBlock.isSuccess()) {
            Bytes::copyBlock(*newBlock.value(), oldBlock, oldSize);
        }
        return newBlock;
    }

    // The operation is an allocation.
    return allocate(newSize, alignment);
}

auto tomurcuk::LinearMemoryAllocator::isLastAllocation(void *block, int64_t size) -> bool {
    if (block == nullptr) {
        return false;
    }
    auto validEnd = (uint64_t)mVirtualBlock.address() + (uint64_t)mCursor;
    auto claimEnd = (uint64_t)block + (uint64_t)size;
    return claimEnd == validEnd;
}

auto tomurcuk::LinearMemoryAllocator::allocate(int64_t size, int64_t alignment) -> Result<void *> {
    auto validEnd = (uint64_t)mVirtualBlock.address() + (uint64_t)mCursor;
    auto reminder = validEnd % (uint64_t)alignment;
    auto padding = INT64_C(0);
    if (reminder != 0) {
        padding = alignment - (int64_t)reminder;
    }

    assert((uint64_t)padding <= UINT64_MAX - validEnd);
    assert(padding <= INT64_MAX - size);

    auto amount = size + padding;
    auto space = mVirtualBlock.load() - mCursor;
    if (space < amount && mVirtualBlock.reserve(amount - space) == Status::eFailure) {
        return Result<void *>::failure();
    }

    auto block = (void *)((char *)mVirtualBlock.address() + mCursor + padding);
    mCursor += amount;
    return Result<void *>::success(block);
}
