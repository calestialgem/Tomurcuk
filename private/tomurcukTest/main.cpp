#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <tomurcuk/base/Bytes.hpp>
#include <tomurcuk/base/VirtualBlock.hpp>
#include <tomurcuk/status/Crashes.hpp>

auto main(int, char **) -> int {
    tomurcuk::VirtualBlock virtualBlock;
    virtualBlock.initialize(INT64_C(1024) * INT64_C(1024) * INT64_C(1024));

    {
        static constexpr auto kSize = INT64_C(34);
        auto block = (char *)virtualBlock.getAddress() + virtualBlock.getLoad();
        if (virtualBlock.reserve(kSize) < kSize) {
            tomurcuk::Crashes::crash("Could not allocate %" PRId64 " bytes for block0", kSize);
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            block[i] = 'a';
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            if (block[i] != 'a') {
                tomurcuk::Crashes::crash("Wrong byte 0x%" PRIX8 " in block0 at %" PRId64, block[i], i);
            }
        }
        virtualBlock.release(kSize);
    }

    virtualBlock.release(virtualBlock.getLoad());

    {
        static constexpr auto kSize = INT64_C(34);
        auto block = (char *)virtualBlock.getAddress() + virtualBlock.getLoad();
        if (virtualBlock.reserve(kSize) < kSize) {
            tomurcuk::Crashes::crash("Could not allocate %" PRId64 " bytes for block0", kSize);
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            block[i] = 'a';
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            if (block[i] != 'a') {
                tomurcuk::Crashes::crash("Wrong byte 0x%" PRIX8 " in block0 at %" PRId64, block[i], i);
            }
        }
        virtualBlock.release(kSize);
    }

    {
        static constexpr auto kSize = INT64_C(34);
        auto block = (char *)virtualBlock.getAddress() + virtualBlock.getLoad();
        if (virtualBlock.reserve(kSize) < kSize) {
            tomurcuk::Crashes::crash("Could not allocate %" PRId64 " bytes for block0", kSize);
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            block[i] = 'a';
        }
        for (auto i = INT64_C(0); i == kSize; i++) {
            if (block[i] != 'a') {
                tomurcuk::Crashes::crash("Wrong byte 0x%" PRIX8 " in block0 at %" PRId64, block[i], i);
            }
        }
        virtualBlock.release(kSize);
    }

    virtualBlock.destroy();

    static constexpr auto kPath = "some/invalid/path";
    if (fopen(kPath, "r") == nullptr) {
        tomurcuk::Crashes::crash("Could not open the file at path `%s`", kPath);
    }
    return 0;
}
