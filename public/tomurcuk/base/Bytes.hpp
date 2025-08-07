#pragma once

#include <stdint.h>

namespace tomurcuk {
    /*
     * Utilities to work with memory without manually calculating byte counts.
     */
    class Bytes {
    public:
        /**
         * Calculates a new capacity that is big enough to hold the current load
         * and the required growth.
         *
         * If the current capacity is big enough, just returns it. Otherwise,
         * amortizes growths by at least doubling the capacity.
         *
         * @param[in] capacity The current amount of elements the array has
         * memory for.
         * @param[in] load The amount of initialized elements in the array.
         * @param[in] reserved The amount of elements the array is required have
         * space for after growth.
         * @return The amount of elements the array should have to hold both the
         * current initialized elements and have required amount of space for
         * new elements.
         */
        static auto growCapacity(int64_t capacity, int64_t load, int64_t reserved) -> int64_t;

        /**
         * Fills elements with `0`s.
         *
         * @tparam Element The type of the elements.
         * @param[out] array The pointer to the array that will be reset.
         * @param[in] count The amount of elements that will be reset.
         */
        template<typename Element>
        static auto resetArray(Element *array, int64_t count) -> void {
            resetBlock(array, count, sizeof(Element));
        }

        /**
         * Fills secret elements with `0`s.
         *
         * This operation cannot be elided by the compiler to ensure the private
         * data is securely erased.
         *
         * @tparam Element The type of the elements.
         * @param[out] array The pointer to the array that will be reset.
         * @param[in] count The amount of elements that will be reset.
         */
        template<typename Element>
        static auto resetSecretArray(Element *array, int64_t count) -> void {
            resetSecretBlock(array, count, sizeof(Element));
        }

        /**
         * Copies elements to another array that is guaranteed to not overlap with it.
         *
         * @tparam Element The type of the elements.
         * @param[out] destinationArray The pointer to the array that will be
         * copied to.
         * @param[in] sourceArray The pointer to the array that will be copied
         * from.
         * @param[in] count The amount of elements that will be copied.
         */
        template<typename Element>
        static auto copyArray(Element *destinationArray, Element *sourceArray, int64_t count) -> void {
            copyBlock(destinationArray, sourceArray, count, sizeof(Element));
        }

        /**
         * Copies elements to another array that might overlap with it.
         *
         * @tparam Element The type of the elements.
         * @param[out] destinationArray The pointer to the array that will be
         * copied to.
         * @param[in] sourceArray The pointer to the array that will be copied
         * from.
         * @param[in] count The amount of elements that will be copied.
         */
        template<typename Element>
        static auto copyAliasingArray(Element *destinationArray, Element *sourceArray, int64_t count) -> void {
            copyAliasingBlock(destinationArray, sourceArray, count, sizeof(Element));
        }

        /**
         * Tests whether elements are exactly the same as another array.
         *
         * @tparam Element The type of the elements.
         * @param[in] array0 The pointer to the first compared array.
         * @param[in] count0 The amount of elements in the first compared array.
         * @param[in] array1 The pointer to the second compared array.
         * @param[in] count1 The amount of elements in the second compared
         * array.
         * @return Whether the elements in the given arrays are exactly the
         * same.
         */
        template<typename Element>
        static auto testArrayExactness(Element *array0, int64_t count0, Element *array1, int64_t count1) -> bool {
            return testBlockExactness(array0, count0, array1, count1, sizeof(Element));
        }

        /**
         * Fills an object with `0`s.
         *
         * @tparam Object The type of the object.
         * @param[out] object The pointer to the object that will be reset.
         */
        template<typename Object>
        static auto resetObject(Object *object) -> void {
            resetBlock(object, 1, sizeof(Object));
        }

        /**
         * Fills a secret object with `0`s.
         *
         * This operation cannot be elided by the compiler to ensure the private
         * data is securely erased.
         *
         * @tparam Object The type of the object.
         * @param[out] object The pointer to the object that will be reset.
         */
        template<typename Object>
        static auto resetSecretObject(Object *object) -> void {
            resetSecretBlock(object, 1, sizeof(Object));
        }

        /**
         * Copies an object to another one that is guaranteed to not overlap with it.
         *
         * @tparam Object The type of the object.
         * @param[out] destinationObject The pointer to the object that will be
         * copied to.
         * @param[in] sourceObject The pointer to the object that will be copied
         * from.
         */
        template<typename Object>
        static auto copyObject(Object *destinationObject, Object *sourceObject) -> void {
            copyBlock(destinationObject, sourceObject, 1, sizeof(Object));
        }

        /**
         * Copies an object to another one that might overlap with it.
         *
         * @tparam Object The type of the object.
         * @param[out] destinationObject The pointer to the object that will be
         * copied to.
         * @param[in] sourceObject The pointer to the object that will be copied
         * from.
         */
        template<typename Object>
        static auto copyAliasingObject(Object *destinationObject, Object *sourceObject) -> void {
            copyAliasingBlock(destinationObject, sourceObject, 1, sizeof(Object));
        }

        /**
         * Tests whether a pair of objects are exactly the same.
         *
         * @tparam Object The type of the object.
         * @param[in] object0 The pointer to the first compared object.
         * @param[in] object1 The pointer to the second compared object.
         * @return Whether the given object are exactly the same.
         */
        template<typename Object>
        static auto testObjectExactness(Object *object0, Object *object1) -> bool {
            return testBlockExactness(object0, 1, object1, 1, sizeof(Object));
        }

    private:
        /**
         * Fills an array of bytes with `0`s.
         *
         * @param[out] block The pointer to the block that will be reset.
         * @param[in] count The amount of elements that will be reset.
         * @param[in] size The amount of bytes in an element.
         */
        static auto resetBlock(void *block, int64_t count, int64_t size) -> void;

        /**
         * Fills a secret array of bytes with `0`s.
         *
         * This operation cannot be elided by the compiler to ensure the private
         * data is securely erased.
         *
         * @param[out] block The pointer to the block that will be reset.
         * @param[in] count The amount of elements that will be reset.
         * @param[in] size The amount of bytes in an element.
         */
        static auto resetSecretBlock(void *block, int64_t count, int64_t size) -> void;

        /**
         * Copies bytes from a block to another one that is guaranteed to not overlap
         * with it.
         *
         * @param[out] destinationBlock The pointer to the block that will be
         * copied to.
         * @param[in] sourceBlock The pointer to the block that will be copied
         * from.
         * @param[in] count The amount of elements that will be copied.
         * @param[in] size The amount of bytes in an element.
         */
        static auto copyBlock(void *destinationBlock, void *sourceBlock, int64_t count, int64_t size) -> void;

        /**
         * Copies bytes from a block to another one that might overlap with it.
         *
         * @param[out] destinationBlock The pointer to the block that will be
         * copied to.
         * @param[in] sourceBlock The pointer to the block that will be copied
         * from.
         * @param[in] count The amount of elements that will be copied.
         * @param[in] size The amount of bytes in an element.
         */
        static auto copyAliasingBlock(void *destinationBlock, void *sourceBlock, int64_t count, int64_t size) -> void;

        /**
         * Tests whether a pair of blocks have the exact same bytes.
         *
         * @param[in] block0 The pointer to the first compared block.
         * @param[in] count0 The amount of elements in the first compared block.
         * @param[in] block1 The pointer to the second compared block.
         * @param[in] count1 The amount of elements in the second compared block.
         * @param[in] size The amount of bytes in an element.
         * @return Whether the bytes in the given blocks are exactly the same.
         */
        static auto testBlockExactness(void *block0, int64_t count0, void *block1, int64_t count1, int64_t size) -> bool;
    };
}
