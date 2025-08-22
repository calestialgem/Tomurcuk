#include <greatest.h>
#include <inttypes.h>
#include <stdint.h>
#include <tomurcuk/Bytes.hpp>
#include <tomurcuk/LinearMemoryAllocator.hpp>
#include <tomurcuk/LinearMemoryAllocatorTest.hpp>
#include <tomurcuk/ObjectOwner.hpp>

auto tomurcuk::LinearMemoryAllocatorTest::suite() -> void {
    GREATEST_RUN_TEST(testAllocating);
    GREATEST_RUN_TEST(testCursor);
    GREATEST_RUN_TEST(testDeallocatingAll);
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

auto tomurcuk::LinearMemoryAllocatorTest::testCursor() -> greatest_test_res {
    static constexpr auto kCapacity = INT64_C(1000);
    static constexpr auto kFirstValue = INT64_C(17);
    static constexpr auto kLoopValue = INT64_C(888'888);
    static constexpr auto kLastValue = INT64_C(55);

    auto linearMemoryAllocatorResult = LinearMemoryAllocator::create(kCapacity);

    GREATEST_ASSERT(linearMemoryAllocatorResult.isSuccess());

    auto linearMemoryAllocator = *linearMemoryAllocatorResult.value();
    auto memoryAllocator = linearMemoryAllocator.memoryAllocator();
    auto firstNumberResult = ObjectOwner<int64_t>::create(memoryAllocator);

    GREATEST_ASSERT(firstNumberResult.isSuccess());

    auto firstNumber = *firstNumberResult.value();
    *firstNumber.get() = kFirstValue;

    auto cursor = linearMemoryAllocator.cursor();
    for (;;) {
        auto numberResult = ObjectOwner<int64_t>::create(memoryAllocator);
        if (numberResult.isFailure()) {
            break;
        }
        auto number = *numberResult.value();
        *number.get() = kLoopValue;
    }
    linearMemoryAllocator.deallocateDownTo(cursor);

    auto lastNumberResult = ObjectOwner<int64_t>::create(memoryAllocator);

    GREATEST_ASSERT(lastNumberResult.isSuccess());

    auto lastNumber = *lastNumberResult.value();
    *lastNumber.get() = kLastValue;

    GREATEST_ASSERT_EQ_FMT(kFirstValue, *firstNumber.get(), "%" PRId64);
    GREATEST_ASSERT_EQ_FMT(kLastValue, *lastNumber.get(), "%" PRId64);

    lastNumber.destroy(memoryAllocator);
    firstNumber.destroy(memoryAllocator);
    linearMemoryAllocator.destroy();

    GREATEST_PASS();
}

auto tomurcuk::LinearMemoryAllocatorTest::testDeallocatingAll() -> greatest_test_res {
    static constexpr auto kCapacity = INT64_C(1000);

    auto linearMemoryAllocatorResult = LinearMemoryAllocator::create(kCapacity);

    GREATEST_ASSERT(linearMemoryAllocatorResult.isSuccess());

    auto linearMemoryAllocator = *linearMemoryAllocatorResult.value();
    auto memoryAllocator = linearMemoryAllocator.memoryAllocator();
    auto maxAllocations = INT64_C(0);
    for (;; maxAllocations++) {
        auto numberResult = ObjectOwner<int64_t>::create(memoryAllocator);
        if (numberResult.isFailure()) {
            break;
        }
    }

    GREATEST_ASSERT(maxAllocations > 0);

    linearMemoryAllocator.deallocateAll();
    auto newAllocations = INT64_C(0);
    for (;; newAllocations++) {
        auto numberResult = ObjectOwner<int64_t>::create(memoryAllocator);
        if (numberResult.isFailure()) {
            break;
        }
    }

    GREATEST_ASSERT_EQ_FMT(maxAllocations, newAllocations, "%" PRId64);

    linearMemoryAllocator.destroy();

    GREATEST_PASS();
}

// NOLINTEND(cert-err33-c,hicpp-signed-bitwise,modernize-use-std-print) cSpell: disable-line
