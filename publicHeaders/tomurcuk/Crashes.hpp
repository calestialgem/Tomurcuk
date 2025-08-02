#pragma once

namespace tomurcuk {
    /**
     * Handles unrecoverable errors.
     */
    class Crashes {
    public:
        /**
         * Halts the program using the current crash handler.
         */
        [[noreturn]]
        static auto crash() -> void;

        /**
         * Sets the current crash handler.
         *
         * @warning This is not thread-safe!
         *
         * @param[in] crashHandler The new value of @ref sCrashHandler.
         */
        static auto setCrashHandler(void (*crashHandler)()) -> void;

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
         */
        static void (*sCrashHandler)();
    };
}
