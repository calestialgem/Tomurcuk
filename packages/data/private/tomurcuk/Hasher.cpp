#include <stdint.h>
#include <tomurcuk/Hasher.hpp>

auto tomurcuk::Hasher::initialize() -> void {
    mValue = 0;
}

auto tomurcuk::Hasher::getValue() -> uint64_t {
    return mValue;
}

auto tomurcuk::Hasher::combine(uint64_t hash) -> void {
    mValue ^= hash + UINT64_C(0x517c'c1b7'2722'0a95) + (mValue << UINT64_C(6)) + (mValue >> UINT64_C(2));
}
