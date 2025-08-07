#include <stdint.h>
#include <tomurcuk/base/VirtualBlock.hpp>

auto tomurcuk::VirtualBlock::getAddress() -> void * {
    return mAddress;
}

auto tomurcuk::VirtualBlock::getLoad() -> int64_t {
    return mLoad;
}
