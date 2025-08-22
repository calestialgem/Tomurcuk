#pragma once

#include <stdint.h>
#include <tomurcuk/Bytes.hpp>

namespace tomurcuk {
    /**
     * Interface of types that can be compared between themselves for equality.
     *
     * @tparam Instance The type that implements this interface.
     */
    template<typename Instance>
    class EqualityComparable {
    public:
        /**
         * Compares the equality of a pair of instances.
         *
         * @param[in] instance0 The first compared instance.
         * @param[in] instance1 The second compared instance.
         * @return Whether the given instances are equal.
         */
        static auto compare(Instance *instance0, Instance *instance1) -> bool = delete;
    };

    template<>
    class EqualityComparable<bool> {
        static auto compare(bool *instance0, bool *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<char> {
        static auto compare(char *instance0, char *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<int8_t> {
        static auto compare(int8_t *instance0, int8_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<int16_t> {
        static auto compare(int16_t *instance0, int16_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<int32_t> {
        static auto compare(int32_t *instance0, int32_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<int64_t> {
        static auto compare(int64_t *instance0, int64_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<uint8_t> {
        static auto compare(uint8_t *instance0, uint8_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<uint16_t> {
        static auto compare(uint16_t *instance0, uint16_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<uint32_t> {
        static auto compare(uint32_t *instance0, uint32_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

    template<>
    class EqualityComparable<uint64_t> {
        static auto compare(uint64_t *instance0, uint64_t *instance1) -> bool {
            return Bytes::testObjectExactness(instance0, instance1);
        }
    };

}
