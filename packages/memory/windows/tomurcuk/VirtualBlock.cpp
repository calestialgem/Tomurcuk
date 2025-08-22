#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/VirtualBlock.hpp>
#include <tomurcuk/Windows.hpp>

static_assert(sizeof(size_t) == 8);
static_assert(sizeof(DWORD) == 4);

auto tomurcuk::VirtualBlock::initialize(int64_t capacity) -> int64_t {
    assert(capacity >= 0);

    if (capacity == 0) {
        mCapacity = alignToAllocationGranularity(1);
    } else {
        mCapacity = alignToAllocationGranularity(capacity);
    }
    mLoad = 0;
    mAddress = VirtualAlloc(nullptr, (size_t)mCapacity, MEM_RESERVE, PAGE_READWRITE);

    if (mAddress == nullptr) {
        return 0;
    }
    return mCapacity;
}

auto tomurcuk::VirtualBlock::destroy() -> void {
    if (VirtualFree(mAddress, 0, MEM_RELEASE) == 0) {
        abort();
    }
}

auto tomurcuk::VirtualBlock::reserve(int64_t amount) -> int64_t {
    assert(amount >= 0);

    auto oldLoad = mLoad;

    if (mLoad > mCapacity - amount) {
        mLoad = mCapacity;
    } else {
        mLoad = alignToAllocationGranularity(mLoad + amount);
    }

    auto actual = mLoad - oldLoad;

    if (actual != 0 && VirtualAlloc((char *)mAddress + oldLoad, (size_t)actual, MEM_COMMIT, PAGE_READWRITE) == nullptr) {
        return -1;
    }

    return actual;
}

auto tomurcuk::VirtualBlock::release(int64_t amount) -> int64_t {
    assert(amount >= 0);

    auto oldLoad = mLoad;

    if (mLoad < amount) {
        mLoad = 0;
    } else {
        mLoad = alignToAllocationGranularity(mLoad - amount);
    }

    auto actual = oldLoad - mLoad;

    if (actual != 0 && VirtualFree((char *)mAddress + mLoad, (size_t)actual, MEM_DECOMMIT) == 0) {
        return -1;
    }

    return actual;
}

auto tomurcuk::VirtualBlock::alignToAllocationGranularity(int64_t amount) -> int64_t {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return Bytes::alignUpwards(amount, (int64_t)systemInfo.dwAllocationGranularity);
}
