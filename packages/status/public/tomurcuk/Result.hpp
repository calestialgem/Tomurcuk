#pragma once

#include <tomurcuk/Status.hpp>

namespace tomurcuk {
    template<typename Value>
    class Result {
    public:
        static auto success(Value value) -> Result<Value> {
            Result<Value> result;
            result.mStatus = Status::eSuccess;
            result.mValue = value;
            return result;
        }

        static auto failure() -> Result<Value> {
            Result<Value> result;
            result.mStatus = Status::eFailure;
            return result;
        }

        auto status() -> Status {
            return mStatus;
        }

        auto isSuccess() -> bool {
            return mStatus == Status::eSuccess;
        }

        auto isFailure() -> bool {
            return mStatus == Status::eFailure;
        }

        auto value() -> Value * {
            return &mValue;
        }

    private:
        Status mStatus;
        Value mValue;
    };
}
