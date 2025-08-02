#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <trk/StandardError.hpp>

auto trk::StandardError::getCurrent() -> StandardError {
    auto result = StandardError{};
    static_assert(sizeof(errno) <= 4);
    result.mCode = (uint32_t)errno;
    return result;
}

auto trk::StandardError::format(char *buffer, size_t capacity) -> size_t {
    if (strerror_s(buffer, capacity, (int)mCode) != 0) {
        return 0;
    }
    return strlen(buffer) + 1;
}

auto trk::StandardError::getCode() -> uint32_t {
    return mCode;
}
