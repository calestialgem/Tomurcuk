#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Results.hpp>
#include <tomurcuk/Status.hpp>

namespace tomurcuk {
    template<typename Element>
    class ArrayOwner {
    public:
        static auto of(Element *pointer) -> ArrayOwner {
            ArrayOwner arrayOwner;
            arrayOwner.mPointer = pointer;
            return arrayOwner;
        }

        static auto null() -> ArrayOwner {
            return of(nullptr);
        }

        static auto create(MemoryAllocator memoryAllocator, int64_t newLength) -> Result<ArrayOwner> {
            assert(newLength >= 0);
            assert(newLength <= INT64_MAX / (int64_t)sizeof(Element));

            auto newBlockResult = memoryAllocator.allocate(newLength * (int64_t)sizeof(Element), alignof(Element));
            if (newBlockResult.isFailure()) {
                return Result<ArrayOwner>::failure();
            }

            return Results::success(of((Element *)*newBlockResult.value()));
        }

        auto destroy(MemoryAllocator memoryAllocator, int64_t oldLength) -> void {
            assert(oldLength >= 0);
            assert(oldLength <= INT64_MAX / (int64_t)sizeof(Element));
            assert((mPointer == nullptr) == (oldLength == 0));

            memoryAllocator.deallocate(mPointer, oldLength * (int64_t)sizeof(Element), alignof(Element));
        }

        auto resize(MemoryAllocator memoryAllocator, int64_t oldLength, int64_t newLength) -> Status {
            assert(oldLength >= 0);
            assert(oldLength <= INT64_MAX / (int64_t)sizeof(Element));
            assert((mPointer == nullptr) == (oldLength == 0));
            assert(newLength >= 0);
            assert(newLength <= INT64_MAX / (int64_t)sizeof(Element));

            auto newBlockResult = memoryAllocator.reallocate(mPointer, oldLength * (int64_t)sizeof(Element), newLength * (int64_t)sizeof(Element), alignof(Element));
            if (newBlockResult.isFailure()) {
                return Status::eFailure;
            }

            mPointer = (Element *)*newBlockResult.value();
            return Status::eSuccess;
        }

        auto isNull() -> bool {
            return mPointer == nullptr;
        }

        auto pointer() -> Element * {
            return mPointer;
        }

        auto get(int64_t index) -> Element * {
            assert(index >= 0);
            assert(mPointer != nullptr);

            return mPointer + index;
        }

    private:
        Element *mPointer;
    };
}
