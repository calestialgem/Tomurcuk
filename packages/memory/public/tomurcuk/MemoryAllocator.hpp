#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/Result.hpp>

namespace tomurcuk {
    class MemoryAllocator {
    public:
        static auto create(void *state, auto (*reallocate)(void *state, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment)->Result<void *>) -> MemoryAllocator;
        auto allocate(int64_t newSize, int64_t alignment) -> Result<void *>;
        auto reallocate(void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *>;
        auto deallocate(void *oldBlock, int64_t oldSize, int64_t alignment) -> void;

    private:
        void *mState;
        auto (*mReallocate)(void *state, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *>;
    };
}
