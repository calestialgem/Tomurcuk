#include <greatest.h>
#include <inttypes.h>
#include <stdint.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/LinearMemoryAllocator.hpp>
#include <tomurcuk/LinearMemoryAllocatorTest.hpp>

auto tomurcuk::LinearMemoryAllocatorTest::suite() -> void {
    GREATEST_RUN_TEST(testAllocating);
    // GREATEST_RUN_TEST(testCursor);
    // GREATEST_RUN_TEST(testDeallocatingAll);
}

// NOLINTBEGIN(cert-err33-c,hicpp-signed-bitwise,modernize-use-std-print) cSpell: disable-line

auto tomurcuk::LinearMemoryAllocatorTest::testAllocating() -> greatest_test_res {
    static constexpr auto kCapacity = INT64_C(1'000'000);
    static constexpr auto kSize = INT64_C(1000);
    static constexpr auto kAlignment = INT64_C(64);

    auto linearMemoryAllocatorResult = LinearMemoryAllocator::create(kCapacity);

    GREATEST_ASSERT(linearMemoryAllocatorResult.isSuccess());

    auto linearMemoryAllocator = *linearMemoryAllocatorResult.value();
    auto memoryAllocator = linearMemoryAllocator.memoryAllocator();
    auto blockResult = memoryAllocator.allocate(kSize, kAlignment);

    GREATEST_ASSERT(blockResult.isSuccess());

    auto block = *blockResult.value();

    GREATEST_ASSERT((uint64_t)block % (uint64_t)kAlignment == 0);

    Bytes::resetBlock(block, kSize);
    memoryAllocator.deallocate(block, kSize, kAlignment);
    linearMemoryAllocator.destroy();

    GREATEST_PASS();
}

// auto tomurcuk::LinearMemoryAllocatorTest::testCursor() -> greatest_test_res {
//     static constexpr auto kCapacity = INT64_C(1000);
//     static constexpr auto kFirstValue = INT64_C(17);
//     static constexpr auto kLoopValue = INT64_C(888'888);
//     static constexpr auto kLastValue = INT64_C(55);

// tomurcuk::LinearMemoryAllocator linearMemoryAllocator;
// auto actualCapacity = linearMemoryAllocator.initialize(kCapacity);

// GREATEST_ASSERT(actualCapacity >= kCapacity);

// auto memoryAllocator = linearMemoryAllocator.getMemoryAllocator();
// int64_t *firstNumber;

// GREATEST_ASSERT(memoryAllocator.allocateObject(&firstNumber));

// *firstNumber = kFirstValue;

// auto cursor = linearMemoryAllocator.getCursor();
// for (int64_t *number; memoryAllocator.allocateObject(&number);) {
//     *number = kLoopValue;
// }
// linearMemoryAllocator.deallocateDownTo(cursor);

// int64_t *lastNumber;

// GREATEST_ASSERT(memoryAllocator.allocateObject(&lastNumber));

// *lastNumber = kLastValue;

// GREATEST_ASSERT_EQ_FMT(kFirstValue, *firstNumber, "%" PRId64);
// GREATEST_ASSERT_EQ_FMT(kLastValue, *lastNumber, "%" PRId64);

// memoryAllocator.deallocateObject(lastNumber);
// memoryAllocator.deallocateObject(firstNumber);
// linearMemoryAllocator.destroy();

// GREATEST_PASS();
// }

// auto tomurcuk::LinearMemoryAllocatorTest::testDeallocatingAll() -> greatest_test_res {
//     static constexpr auto kCapacity = INT64_C(1000);

// tomurcuk::LinearMemoryAllocator linearMemoryAllocator;
// auto actualCapacity = linearMemoryAllocator.initialize(kCapacity);

// GREATEST_ASSERT(actualCapacity >= kCapacity);

// auto memoryAllocator = linearMemoryAllocator.getMemoryAllocator();
// auto maxAllocations = INT64_C(0);
// for (int64_t *number; memoryAllocator.allocateObject(&number); maxAllocations++) {
// }

// GREATEST_ASSERT(maxAllocations > 0);

// linearMemoryAllocator.deallocateAll();
// auto newAllocations = INT64_C(0);
// for (int64_t *number; memoryAllocator.allocateObject(&number); newAllocations++) {
// }

// GREATEST_ASSERT_EQ_FMT(maxAllocations, newAllocations, "%" PRId64);

// linearMemoryAllocator.destroy();

// GREATEST_PASS();
// }

// NOLINTEND(cert-err33-c,hicpp-signed-bitwise,modernize-use-std-print) cSpell: disable-line
