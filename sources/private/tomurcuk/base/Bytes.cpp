#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tomurcuk/base/Bytes.hpp>

static_assert(sizeof(size_t) == 8);

auto tomurcuk::Bytes::alignUpwards(int64_t amount, int64_t alignment) -> int64_t {
    assert(amount >= 0);
    assert(alignment > 0);

    auto reminder = amount % alignment;
    if (reminder != 0) {
        auto padding = alignment - reminder;
        if (amount > INT64_MAX - padding) {
            abort();
        }
        amount += padding;
    }
    return amount;
}

auto tomurcuk::Bytes::growCapacity(int64_t capacity, int64_t load, int64_t reserved) -> int64_t {
    assert(capacity >= 0);
    assert(load >= 0);
    assert(load <= capacity);
    assert(reserved >= 0);

    if (reserved > INT64_MAX - load) {
        abort();
    }
    auto requiredCapacity = load + reserved;

    if (requiredCapacity <= capacity) {
        return capacity;
    }

    auto amortizedCapacity = INT64_MAX;
    if (capacity <= INT64_MAX / 2) {
        amortizedCapacity = capacity * 2;
    }

    if (requiredCapacity < amortizedCapacity) {
        return amortizedCapacity;
    }
    return requiredCapacity;
}

auto tomurcuk::Bytes::resetBlock(void *block, int64_t size) -> void {
    assert((block == nullptr) == (size == 0));
    assert(size >= 0);

    memset(block, 0, (size_t)size);
}

auto tomurcuk::Bytes::copyBlock(void *destinationBlock, void *sourceBlock, int64_t size) -> void {
    assert((destinationBlock == nullptr) == (size == 0));
    assert((sourceBlock == nullptr) == (size == 0));
    assert(size >= 0);

    memcpy(destinationBlock, sourceBlock, (size_t)size);
}

auto tomurcuk::Bytes::copyAliasingBlock(void *destinationBlock, void *sourceBlock, int64_t size) -> void {
    assert((destinationBlock == nullptr) == (size == 0));
    assert((sourceBlock == nullptr) == (size == 0));
    assert(size >= 0);

    memmove(destinationBlock, sourceBlock, (size_t)size);
}

auto tomurcuk::Bytes::testBlockExactness(void *block0, int64_t size0, void *block1, int64_t size1) -> bool {
    assert((block0 == nullptr) == (size0 == 0));
    assert((block1 == nullptr) == (size1 == 0));
    assert(size0 >= 0);
    assert(size1 >= 0);

    if (size0 != size1) {
        return false;
    }

    return memcmp(block0, block1, (size_t)size0) == 0;
}
