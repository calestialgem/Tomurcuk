#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/MemoryAllocator.hpp>

auto tomurcuk::MemoryAllocator::initialize(void *state, auto (*reallocate)(void *state, void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment)->bool) -> void {
    assert(reallocate != nullptr);

    mState = state;
    mReallocate = reallocate;
}

auto tomurcuk::MemoryAllocator::allocateBlock(void **newBlock, int64_t newSize, int64_t alignment) -> bool {
    return mReallocate(mState, newBlock, nullptr, 0, newSize, alignment);
}

auto tomurcuk::MemoryAllocator::reallocateBlock(void **newBlock, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> bool {
    return mReallocate(mState, newBlock, oldBlock, oldSize, newSize, alignment);
}

auto tomurcuk::MemoryAllocator::deallocateBlock(void *oldBlock, int64_t oldSize, int64_t alignment) -> void {
    void *newBlock;
    auto status = mReallocate(mState, &newBlock, oldBlock, oldSize, 0, alignment);
    if (!status) {
        abort();
    }
}
