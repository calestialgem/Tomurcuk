#pragma once

#include <stdint.h>

namespace tomurcuk {
    /**
     * Error code from the operating system.
     */
    class PlatformError {
    public:
        /**
         * Gets the current thread-local error code.
         *
         * @return The error code of the current thread.
         */
        static auto getCurrent() -> PlatformError;

        /**
         * Formats the error message to a buffer.
         *
         * Puts a null-termination sentinel to the end.
         *
         * @warning Encoding of the resulting message depends on the platform.
         *
         * @param[out] buffer The memory that will hold the formatting result.
         * @param[in] capacity The maximum amount of bytes that might be written
         * into `buffer`, including the null-termination sentinel.
         * @return The amount of bytes that were written into `buffer`,
         * including the null-termination sentinel on success. Otherwise, zero.
         */
        auto format(char *buffer, uint64_t capacity) -> uint64_t;

        /**
         * Provides the error as a 32-bit integer.
         *
         * @return @ref mCode.
         */
        auto getCode() -> uint32_t;

    private:
        /**
         * Integer that represents the error.
         */
        uint32_t mCode;
    };
}
