#include <stdint.h>
#include <tomurcuk/base/windows/Windows.hpp>
#include <tomurcuk/status/PlatformError.hpp>

auto tomurcuk::PlatformError::getCurrent() -> PlatformError {
    auto result = PlatformError{};
    static_assert(sizeof(DWORD) <= 4);
    result.mCode = (uint32_t)GetLastError();
    return result;
}

auto tomurcuk::PlatformError::format(char *buffer, uint64_t capacity) -> uint64_t {
    // `FormatMessage` takes a `DWORD`, make sure the capacity is limited to
    // that.
    uint32_t trimmedCapacity = UINT32_MAX;
    if (capacity <= UINT32_MAX) {
        trimmedCapacity = (uint32_t)capacity;
    }

    // Try with `en_US` language.
    auto status = FormatMessageA(
            FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr,
            mCode,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            buffer,
            trimmedCapacity,
            nullptr
    );
    if (status != 0) {
        return (uint64_t)status + 1;
    }

    // If failure was not caused by the language, report it.
    if (GetLastError() != ERROR_RESOURCE_LANG_NOT_FOUND) {
        return 0;
    }

    // Try with the default language.
    status = FormatMessageA(
            FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
            nullptr,
            mCode,
            0,
            buffer,
            trimmedCapacity,
            nullptr
    );
    if (status != 0) {
        return (uint64_t)status + 1;
    }

    // Failed again, just report.
    return 0;
}

auto tomurcuk::PlatformError::getCode() -> uint32_t {
    return mCode;
}
