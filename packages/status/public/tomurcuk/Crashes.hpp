#pragma once

#include <stdarg.h>
#include <stdint.h>

namespace tomurcuk {
    /**
     * Handles unrecoverable errors.
     */
    class Crashes {
    public:
        /**
         * Version of @ref crashWith that directly takes arguments.
         *
         * @param[in] format The specification that runs the formatting. Must
         * obey C standard library formatting rules.
         * @param[in] ... The arguments to be formatted.
         */
        [[noreturn, gnu::format(printf, 1, 2)]]
        static auto crash(char *format, ...) -> void;

        /**
         * Halts the program using the current crash handler after reporting the
         * given formatted message, and current values of @ref PlatformError and
         * @ref StandardError.
         *
         * @param[in] format The specification that runs the formatting. Must
         * obey C standard library formatting rules.
         * @param[in] arguments The arguments to be formatted.
         */
        [[noreturn, gnu::format(printf, 1, 0)]]
        static auto crashWith(char *format, va_list arguments) -> void;

        /**
         * Sets the current crash handler.
         *
         * @warning This is not thread-safe!
         *
         * @param[in] crashHandler The new value of @ref sCrashHandler.
         */
        static auto setCrashHandler(auto (*crashHandler)()->void) -> void;

    private:
        /**
         * The pointer to the function that will be called to terminate the
         * program.
         *
         * Might be `nullptr`, which will work the same as an empty function.
         *
         * If the handler does not terminate, `abort` will be called.
         *
         * @warning Must be reentrant if it might call @ref crash recursively.
         *
         * @warning Must be synchronized if @ref crash might be called
         * concurrently.
         */
        static auto (*sCrashHandler)() -> void;

        /**
         * Removes any trailing new lines from the given buffer.
         *
         * Preserves the null-termination sentinel if it previously existed.
         *
         * @warning Encoding must be ASCII compatible.
         *
         * @param[in,out] buffer The modified buffer.
         * @param[in] load The amount of bytes in the buffer, including the
         * null-termination sentinel.
         * @return The new amount of bytes in the buffer.
         */
        static auto removeTrailingNewLines(char *buffer, uint64_t load) -> uint64_t;
    };
}
