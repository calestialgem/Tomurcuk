#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <tomurcuk/StandardError.hpp>

auto tomurcuk::StandardError::getCurrent() -> StandardError {
    auto result = StandardError{};
    static_assert(sizeof(errno) <= 4);
    result.mCode = (uint32_t)errno;
    return result;
}

auto tomurcuk::StandardError::format(char *buffer, uint64_t capacity) -> uint64_t {
    if (strerror_s(buffer, capacity, (int)mCode) != 0) {
        return 0;
    }
    return strlen(buffer) + 1;
}

auto tomurcuk::StandardError::getCode() -> uint32_t {
    return mCode;
}
