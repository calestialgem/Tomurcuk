#include <stdint.h>
#include <tomurcuk/VirtualBlock.hpp>

auto tomurcuk::VirtualBlock::address() -> void * {
    return mAddress;
}

auto tomurcuk::VirtualBlock::load() -> int64_t {
    return mLoad;
}
