#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/Windows.hpp>

static_assert(sizeof(size_t) == 8);

auto tomurcuk::Bytes::resetSecretBlock(void *block, int64_t size) -> void {
    assert((block == nullptr) == (size == 0));
    assert(size >= 0);

    SecureZeroMemory(block, (size_t)size);
}
