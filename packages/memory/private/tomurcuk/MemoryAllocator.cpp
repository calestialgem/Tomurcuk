#include <assert.h>
#include <stdint.h>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/Result.hpp>

auto tomurcuk::MemoryAllocator::create(void *state, auto (*reallocate)(void *state, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment)->Result<void *>) -> MemoryAllocator {
    assert(reallocate != nullptr);

    MemoryAllocator memoryAllocator;
    memoryAllocator.mState = state;
    memoryAllocator.mReallocate = reallocate;
    return memoryAllocator;
}

auto tomurcuk::MemoryAllocator::allocate(int64_t newSize, int64_t alignment) -> Result<void *> {
    return mReallocate(mState, nullptr, 0, newSize, alignment);
}

auto tomurcuk::MemoryAllocator::reallocate(void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *> {
    return mReallocate(mState, oldBlock, oldSize, newSize, alignment);
}

auto tomurcuk::MemoryAllocator::deallocate(void *oldBlock, int64_t oldSize, int64_t alignment) -> void {
    auto result = mReallocate(mState, oldBlock, oldSize, 0, alignment);
    assert(result.isSuccess());
    assert(result.value() == nullptr);
    (void)result;
}
