#pragma once

#include <stdint.h>
#include <tomurcuk/Bytes.hpp>

namespace tomurcuk {
    /**
     * Tool that combines hash values.
     */
    class Hasher {
    public:
        /**
         * Creates a new hasher with value `0`.
         */
        auto initialize() -> void;

        /**
         * Provides the current value.
         *
         * @return The current value of the hash.
         */
        auto getValue() -> uint64_t;

        /**
         * Combines a hash value with the current one.
         *
         * @param[in] hash The hash that is combined on top of the current one.
         */
        auto combine(uint64_t hash) -> void;

    private:
        /**
         * Current value of the hash.
         */
        uint64_t mValue;
    };
}
