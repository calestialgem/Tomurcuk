#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <trk/StandardError.hpp>

auto trk::StandardError::getCurrent() -> StandardError {
    auto result = StandardError{};
    static_assert(sizeof(result.mValue) == sizeof(errno));
    result.mValue = errno;
    return result;
}

auto trk::StandardError::format(char *buffer, size_t capacity) -> size_t {
    if (strerror_s(buffer, capacity, mValue) != 0) {
        return 0;
    }
    return strlen(buffer) + 1;
}
