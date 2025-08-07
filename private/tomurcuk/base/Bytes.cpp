#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tomurcuk/base/Bytes.hpp>

static_assert(sizeof(size_t) == 8);

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

auto tomurcuk::Bytes::resetBlock(void *block, int64_t count, int64_t size) -> void {
    assert((block == nullptr) == (count == 0));
    assert(count >= 0);
    assert(size > 0);

    if (count > INT64_MAX / size) {
        abort();
    }
    auto bytes = count * size;

    memset(block, 0, (size_t)bytes);
}

auto tomurcuk::Bytes::copyBlock(void *destinationBlock, void *sourceBlock, int64_t count, int64_t size) -> void {
    assert((destinationBlock == nullptr) == (count == 0));
    assert((sourceBlock == nullptr) == (count == 0));
    assert(count >= 0);
    assert(size > 0);

    if (count > INT64_MAX / size) {
        abort();
    }
    auto bytes = count * size;

    memcpy(destinationBlock, sourceBlock, (size_t)bytes);
}

auto tomurcuk::Bytes::copyAliasingBlock(void *destinationBlock, void *sourceBlock, int64_t count, int64_t size) -> void {
    assert((destinationBlock == nullptr) == (count == 0));
    assert((sourceBlock == nullptr) == (count == 0));
    assert(count >= 0);
    assert(size > 0);

    if (count > INT64_MAX / size) {
        abort();
    }
    auto bytes = count * size;

    memmove(destinationBlock, sourceBlock, (size_t)bytes);
}

auto tomurcuk::Bytes::testBlockExactness(void *block0, int64_t count0, void *block1, int64_t count1, int64_t size) -> bool {
    assert((block0 == nullptr) == (count0 == 0));
    assert((block1 == nullptr) == (count1 == 0));
    assert(count0 >= 0);
    assert(count1 >= 0);
    assert(size > 0);

    if (count0 != count1) {
        return false;
    }

    if (count0 > INT64_MAX / size) {
        abort();
    }
    auto bytes = count0 * size;

    return memcmp(block0, block1, (size_t)bytes) == 0;
}
