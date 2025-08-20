#pragma once

namespace tomurcuk {
    /**
     * Showcases the private types.
     */
    class PrivateDemo {
    public:
        /**
         * Demonstrates the private functionality.
         *
         * @return A pointer to a null-terminated string.
         */
        static auto message() -> char *;
    };
}
