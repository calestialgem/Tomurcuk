#pragma once

#include <tomurcuk/Result.hpp>

namespace tomurcuk {
    class Results {
    public:
        template<typename Value>
        static auto success(Value value) -> Result<Value> {
            return Result<Value>::success(value);
        }
    };
}
