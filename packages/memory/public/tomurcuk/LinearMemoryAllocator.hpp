#pragma once

#include <stdint.h>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Status.hpp>
#include <tomurcuk/VirtualBlock.hpp>

namespace tomurcuk {
    class LinearMemoryAllocator {
    public:
        static auto create(int64_t capacity) -> Result<LinearMemoryAllocator>;
        auto destroy() -> void;
        auto memoryAllocator() -> MemoryAllocator;
        auto cursor() -> int64_t;
        auto deallocateDownTo(int64_t cursor) -> void;
        auto deallocateAll() -> void;
        auto releaseUnused() -> Status;

    private:
        static auto reallocateImplementation(void *state, void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *>;
        auto reallocate(void *oldBlock, int64_t oldSize, int64_t newSize, int64_t alignment) -> Result<void *>;
        auto isLastAllocation(void *block, int64_t size) -> bool;
        auto allocate(int64_t size, int64_t alignment) -> Result<void *>;

        VirtualBlock mVirtualBlock;
        int64_t mCursor;
    };
}
