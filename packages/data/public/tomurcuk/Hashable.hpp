#pragma once

#include <stdint.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/Hasher.hpp>

namespace tomurcuk {
    /**
     * Interface of types that can be converted to a hash value.
     *
     * @tparam Instance The type that implements this interface.
     */
    template<typename Instance>
    class Hashable {
    public:
        /**
         * Hashes an instance via a newly created hasher.
         *
         * @warning Do not use this when implementing this interface. Use the
         * overload that takes a @ref Hasher when hashing subvalues.
         *
         * @param[in] instance The hashed instance.
         * @return The found hash value.
         */
        static auto hash(Instance *instance) -> uint64_t {
            Hasher hasher;
            hasher.initialize();
            hash(&hasher, instance);
            return hasher.getValue();
        }

        /**
         * Hashes an instance via a hasher.
         *
         * @warning Do not call this function from the end-consumer of hash
         * values. Those should not need to create a @ref Hasher explicitly.
         *
         * @param[in,out] hasher The tool that combines the hashes of subvalues.
         * @param[in] instance The hashed instance.
         */
        static auto hash(Hasher *hasher, Instance *instance) -> void = delete;
    };

    template<>
    class Hashable<bool> {
        static auto hash(Hasher *hasher, bool *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<char> {
        static auto hash(Hasher *hasher, char *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<int8_t> {
        static auto hash(Hasher *hasher, int8_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<int16_t> {
        static auto hash(Hasher *hasher, int16_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<int32_t> {
        static auto hash(Hasher *hasher, int32_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<int64_t> {
        static auto hash(Hasher *hasher, int64_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<uint8_t> {
        static auto hash(Hasher *hasher, uint8_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<uint16_t> {
        static auto hash(Hasher *hasher, uint16_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<uint32_t> {
        static auto hash(Hasher *hasher, uint32_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };

    template<>
    class Hashable<uint64_t> {
        static auto hash(Hasher *hasher, uint64_t *instance) -> void {
            auto hash = UINT64_C(0);
            Bytes::copyBlock(&hash, instance, sizeof(*instance));
            hasher->combine(hash);
        }
    };
}
