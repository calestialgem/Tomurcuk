#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Status.hpp>
#include <tomurcuk/VirtualBlock.hpp>
#include <tomurcuk/Windows.hpp>

static_assert(sizeof(size_t) == 8);
static_assert(sizeof(DWORD) == 4);

auto tomurcuk::VirtualBlock::create(int64_t capacity) -> Result<VirtualBlock> {
    assert(capacity >= 0);

    if (capacity == 0) {
        capacity = alignToAllocationGranularity(1);
    } else {
        capacity = alignToAllocationGranularity(capacity);
    }

    auto address = VirtualAlloc(nullptr, (size_t)capacity, MEM_RESERVE, PAGE_READWRITE);
    if (address == nullptr) {
        return Result<VirtualBlock>::failure();
    }

    VirtualBlock virtualBlock;
    virtualBlock.mAddress = address;
    virtualBlock.mCapacity = capacity;
    virtualBlock.mLoad = 0;
    return Result<VirtualBlock>::success(virtualBlock);
}

auto tomurcuk::VirtualBlock::destroy() -> void {
    if (VirtualFree(mAddress, 0, MEM_RELEASE) == 0) {
        abort();
    }
}

auto tomurcuk::VirtualBlock::reserve(int64_t amount) -> Status {
    assert(amount >= 0);

    if (mLoad > mCapacity - amount) {
        return Status::eFailure;
    }

    auto newLoad = alignToAllocationGranularity(mLoad + amount);
    auto actualAmount = newLoad - mLoad;
    if (actualAmount != 0 && VirtualAlloc((char *)mAddress + mLoad, (size_t)actualAmount, MEM_COMMIT, PAGE_READWRITE) == nullptr) {
        return Status::eFailure;
    }

    mLoad = newLoad;
    return Status::eSuccess;
}

auto tomurcuk::VirtualBlock::release(int64_t amount) -> Status {
    assert(amount >= 0);
    assert(amount <= mLoad);

    auto newLoad = alignToAllocationGranularity(mLoad - amount);
    auto actualAmount = mLoad - newLoad;
    if (actualAmount != 0 && VirtualFree((char *)mAddress + newLoad, (size_t)actualAmount, MEM_DECOMMIT) == 0) {
        return Status::eFailure;
    }

    mLoad = newLoad;
    return Status::eSuccess;
}

auto tomurcuk::VirtualBlock::alignToAllocationGranularity(int64_t amount) -> int64_t {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return Bytes::alignUpwards(amount, (int64_t)systemInfo.dwAllocationGranularity);
}
