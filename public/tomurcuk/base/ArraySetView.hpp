#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/base/EqualityComparable.hpp>
#include <tomurcuk/base/Hashable.hpp>

namespace tomurcuk {
    /**
     * Reference to a contiguous block of memory that holds elements and buckets
     * that form a hash table over them.
     *
     * @tparam Element The type of the elements.
     *
     * @warning Reference might not be a view of any array. That case should act
     * as if an empty array was viewed.
     */
    template<typename Element>
    class ArraySetView {
    public:
        /**
         * Creates as reference to arrays.
         *
         * @param[in] array The referred array of elements.
         * @param[in] hashArray The referred array of cached hash values.
         * @param[in] count The amount of referred elements.
         * @param[in] bucketArray The referred array of buckets.
         * @param[in] bucketCount The amount of referred buckets.
         */
        auto initialize(Element *array, uint64_t *hashArray, int64_t count, int64_t *bucketArray, int64_t bucketCount) -> void {
            assert((array == nullptr) == (count == 0));
            assert((hashArray == nullptr) == (count == 0));
            assert(count >= 0);
            assert((bucketArray == nullptr) == (bucketCount == 0));
            assert(bucketCount >= 0);

            mArray = array;
            mHashArray = hashArray;
            mCount = count;
            mBucketArray = bucketArray;
            mBucketCount = bucketCount;
        }

        /**
         * Creates an empty view.
         */
        auto initializeEmpty() -> void {
            mArray = nullptr;
            mHashArray = nullptr;
            mCount = 0;
            mBucketArray = nullptr;
            mBucketCount = 0;
        }

        /**
         * Provides the referred array of elements.
         *
         * @return The pointer to the referred array of elements if it exists.
         * Otherwise, `nullptr`.
         */
        auto getArray() -> Element * {
            return mArray;
        }

        /**
         * Provides the amount of referred elements.
         *
         * @return The amount of referred elements.
         */
        auto getCount() -> int64_t {
            return mCount;
        }

        /**
         * Provides the amount of referred bytes.
         *
         * @return The total amount of bytes that constitute the referred
         * elements.
         */
        auto getSize() -> int64_t {
            assert(mCount <= INT64_MAX / (int64_t)sizeof(Element));

            return mCount * (int64_t)sizeof(Element);
        }

        /**
         * Tests whether there are no elements.
         *
         * @return Whether there are no elements in the view.
         */
        auto isEmpty() -> bool {
            return mCount == 0;
        }

        /**
         * Provides the pointer to the first element.
         *
         * @return The pointer to the first element if there is one. Otherwise,
         * the pointer to the referred array of elements.
         */
        auto getFirst() -> Element * {
            return mArray;
        }

        /**
         * Provides the pointer to the last element.
         *
         * @return The pointer to the last element if there is one. Otherwise,
         * the pointer to the referred array of elements.
         */
        auto getLast() -> Element * {
            return mArray + mCount - (mCount != 0);
        }

        /**
         * Provides the pointer to the element at an index.
         *
         * @param[in] index The amount of elements before the accessed element.
         * @return The pointer to the element at the given index.
         */
        auto get(int64_t index) -> Element * {
            assert(index >= 0);
            assert(index < mCount);

            return mArray[index];
        }

        /**
         * Queries an element's equivalent's membership.
         *
         * @param[in] queriedElement The queried element.
         * @return The given element's equivalent's index if it exists.
         * Otherwise, `-1`.
         */
        auto locate(Element *queriedElement) -> int64_t {
            if (mBucketCount == 0) {
                return -1;
            }

            auto queriedHash = Hashable<Element>::hash(queriedElement);

            for (auto queriedProbeLength = INT64_C(0);; queriedProbeLength++) {
                // Hypothetical bucket index of the queried element, which is
                // also the bucket index of the tested element.
                auto bucketIndex = findBucketIndex(queriedHash, queriedProbeLength);

                // If there is no element to test, it is not in.
                auto testedIndex = mBucketArray[bucketIndex];
                if (testedIndex == -1) {
                    return -1;
                }

                // If the tested element compares equal, we have found it.
                auto testedHash = mHashArray[testedIndex];
                auto testedElement = mArray + testedIndex;
                if (queriedHash == testedHash && EqualityComparable<Element>::compare(queriedElement, testedElement)) {
                    return testedIndex;
                }

                // If we have surpassed the probe length of the tested element,
                // the queried element could not be any further. Because, it
                // would have evicted the tested element to a further bucket.
                auto testedProbeLength = findProbeLength(testedHash, bucketIndex);
                if (queriedProbeLength > testedProbeLength) {
                    return -1;
                }
            }
        }

    private:
        /**
         * Finds the hypothetical bucket index that corresponds to the element
         * with a hash, when it has a particular probe length.
         *
         * @param[in] hash The element's hash.
         * @param[in] probeLength The element's distance from its preferred
         * bucket index.
         * @return The hypothetical bucket index of the element.
         */
        auto findBucketIndex(uint64_t hash, int64_t probeLength) -> int64_t {
            // Do a signed modulus to protect against overflow. Then, handle the
            // sign by adding the modulus once if the result is negative.
            auto reminder = ((int64_t)hash % mBucketCount + probeLength) % mBucketCount;
            return reminder + (int64_t)((uint64_t)mBucketCount & (uint64_t)-(int64_t)(reminder < 0));
        }

        /**
         * Finds the hypothetical probe length that corresponds to the element
         * with a hash, when it is at a particular bucket index.
         *
         * @param[in] hash The element's hash.
         * @param[in] bucketIndex The index of the bucket that holds the
         * element.
         * @return The hypothetical probe length of the element.
         */
        auto findProbeLength(uint64_t hash, int64_t bucketIndex) -> int64_t {
            // Do a signed modulus to protect against overflow. Then, handle the
            // sign by adding the modulus once if the result is negative.
            auto reminder = (bucketIndex - (int64_t)hash % mBucketCount) % mBucketCount;
            return reminder + (int64_t)((uint64_t)mBucketCount & (uint64_t)-(int64_t)(reminder < 0));
        }

        /**
         * Pointer to the referred array of elements.
         *
         * @warning Might be `nullptr` if there are no elements.
         */
        Element *mArray;

        /**
         * Pointer to the referred array of cached hash values.
         *
         * @warning Might be `nullptr` if there are no elements.
         */
        uint64_t *mHashArray;

        /**
         * The amount of referred elements.
         */
        int64_t mCount;

        /**
         * Pointer to the referred array of buckets.
         *
         * @warning Might be `nullptr` if there are no buckets.
         */
        int64_t *mBucketArray;

        /**
         * The amount of referred buckets.
         */
        int64_t mBucketCount;
    };
}
