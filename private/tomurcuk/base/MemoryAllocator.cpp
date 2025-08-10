#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/base/MemoryAllocator.hpp>

auto tomurcuk::MemoryAllocator::initialize(void *state, bool (*reallocate)(void *state, void **newBlock, void *oldBlock, int64_t oldCount, int64_t newCount, int64_t size, int64_t alignment)) -> void {
    assert(reallocate != nullptr);

    mState = state;
    mReallocate = reallocate;
}

auto tomurcuk::MemoryAllocator::allocateBlock(void **newBlock, int64_t newCount, int64_t size, int64_t alignment) -> bool {
    return mReallocate(mState, newBlock, nullptr, 0, newCount, size, alignment);
}

auto tomurcuk::MemoryAllocator::reallocateBlock(void **newBlock, void *oldBlock, int64_t oldCount, int64_t newCount, int64_t size, int64_t alignment) -> bool {
    return mReallocate(mState, newBlock, oldBlock, oldCount, newCount, size, alignment);
}

auto tomurcuk::MemoryAllocator::deallocateBlock(void *oldBlock, int64_t oldCount, int64_t size, int64_t alignment) -> void {
    void *newBlock;
    auto status = mReallocate(mState, &newBlock, oldBlock, oldCount, 0, size, alignment);
    if (!status) {
        abort();
    }
}
