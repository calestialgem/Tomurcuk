#pragma once

#include <stdint.h>

namespace tomurcuk {
    /**
     * Block of virtual bytes that are mapped to real bytes as needed.
     *
     * The block has finitely many bytes, which are sliced into two in order:
     * mapped and unmapped.
     */
    class VirtualBlock {
    public:
        /**
         * Creates a new virtual mapping with a capacity.
         *
         * @param[in] capacity The amount of virtual bytes that will be
         * allocated. This is rounded up to the closest platform supported
         * amount that is positive.
         * @return The actually allocated amount of virtual bytes on success. On
         * failure, `0`.
         */
        auto initialize(int64_t capacity) -> int64_t;

        /**
         * Deallocates all the virtual bytes.
         */
        auto destroy() -> void;

        /**
         * Provides the address of the mapped region.
         *
         * @return The pointer to the first mapped byte if there are any.
         * Otherwise, the pointer to the first unmapped byte.
         */
        auto getAddress() -> void *;

        /**
         * Provides the total amount of mapped bytes.
         *
         * @return The total amount of mapped bytes.
         */
        auto getLoad() -> int64_t;

        /**
         * Maps virtual bytes to real bytes.
         *
         * @warning The actually mapped amount might be lower than requested.
         *
         * @param[in] amount The amount of virtual bytes that will be mapped.
         * This is rounded up to the closest platform supported amount if there
         * is enough space. Otherwise, it is rounded down to consume all the
         * remaining virtual bytes.
         * @return The actually mapped amount of bytes on success. On failure,
         * `-1`.
         */
        auto reserve(int64_t amount) -> int64_t;

        /**
         * Unmaps virtual bytes from real bytes.
         *
         * @param[in] amount The amount of virtual bytes that will be unmapped.
         * This is rounded down to the closest platform supported amount.
         * @return The actually unmapped amount of bytes on success. On failure,
         * `-1`.
         */
        auto release(int64_t amount) -> int64_t;

    private:
        /**
         * Aligns an amount of bytes up to the virtual allocation granularity of
         * the underlying platform.
         *
         * @param[in] amount The aligned amount.
         * @return The correctly aligned amount.
         */
        static auto alignToAllocationGranularity(int64_t amount) -> int64_t;

        /**
         * Address of the virtual memory region.
         */
        void *mAddress;

        /**
         * Amount of virtual bytes in the block.
         */
        int64_t mCapacity;

        /**
         * Amount of real bytes that are mapped to the block.
         */
        int64_t mLoad;
    };
}
