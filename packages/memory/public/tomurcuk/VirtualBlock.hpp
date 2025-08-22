#pragma once

#include <stdint.h>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Status.hpp>

namespace tomurcuk {
    class VirtualBlock {
    public:
        static auto create(int64_t capacity) -> Result<VirtualBlock>;
        auto destroy() -> void;
        auto address() -> void *;
        auto load() -> int64_t;
        auto reserve(int64_t amount) -> Status;
        auto release(int64_t amount) -> Status;

    private:
        static auto alignToAllocationGranularity(int64_t amount) -> int64_t;

        void *mAddress;
        int64_t mCapacity;
        int64_t mLoad;
    };
}
