#include <greatest.h>
#include <inttypes.h>
#include <stdint.h>
#include <tomurcuk/ArrayOwner.hpp>
#include <tomurcuk/ArrayOwnerTest.hpp>
#include <tomurcuk/LinearMemoryAllocator.hpp>
#include <tomurcuk/Status.hpp>

auto tomurcuk::ArrayOwnerTest::suite() -> void {
    GREATEST_RUN_TEST(testResizing);
}

// NOLINTBEGIN(cert-err33-c,hicpp-signed-bitwise,modernize-use-std-print) cSpell: disable-line

auto tomurcuk::ArrayOwnerTest::testResizing() -> greatest_test_res {
    static constexpr auto kCapacity = INT64_C(1'000'000);
    static constexpr auto kLength = INT64_C(1024);
    static constexpr auto kBiggerLength = kLength + 1;
    static constexpr auto kSmallerLength = kLength - 1;

    auto linearMemoryAllocatorResult = LinearMemoryAllocator::create(kCapacity);

    GREATEST_ASSERT(linearMemoryAllocatorResult.isSuccess());

    auto linearMemoryAllocator = *linearMemoryAllocatorResult.value();
    auto memoryAllocator = linearMemoryAllocator.memoryAllocator();
    auto arrayOwnerResult = ArrayOwner<int64_t>::create(memoryAllocator, kLength);

    GREATEST_ASSERT(arrayOwnerResult.isSuccess());

    auto arrayOwner = *arrayOwnerResult.value();
    for (auto i = INT64_C(0); i != kLength; i++) {
        *arrayOwner.get(i) = i;
    }

    for (auto i = INT64_C(0); i != kLength; i++) {
        GREATEST_ASSERT_EQ_FMT(i, *arrayOwner.get(i), "%" PRId64);
    }

    GREATEST_ASSERT(arrayOwner.resize(memoryAllocator, kLength, kBiggerLength) == Status::eSuccess);

    for (auto i = INT64_C(0); i != kLength; i++) {
        GREATEST_ASSERT_EQ_FMT(i, *arrayOwner.get(i), "%" PRId64);
    }

    GREATEST_ASSERT(arrayOwner.resize(memoryAllocator, kBiggerLength, kSmallerLength) == Status::eSuccess);

    for (auto i = INT64_C(0); i != kSmallerLength; i++) {
        GREATEST_ASSERT_EQ_FMT(i, *arrayOwner.get(i), "%" PRId64);
    }

    arrayOwner.destroy(memoryAllocator, kSmallerLength);
    linearMemoryAllocator.destroy();

    GREATEST_PASS();
}

// NOLINTEND(cert-err33-c,hicpp-signed-bitwise,modernize-use-std-print) cSpell: disable-line
