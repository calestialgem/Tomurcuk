#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <tomurcuk/base/Bytes.hpp>
#include <tomurcuk/base/windows/Windows.hpp>

static_assert(sizeof(size_t) == 8);

auto tomurcuk::Bytes::resetSecretBlock(void *block, int64_t size) -> void {
    assert((block == nullptr) == (size == 0));
    assert(size >= 0);

    SecureZeroMemory(block, (size_t)size);
}
