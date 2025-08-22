#pragma once

#include <stdint.h>
#include <tomurcuk/MemoryAllocator.hpp>
#include <tomurcuk/Result.hpp>
#include <tomurcuk/Results.hpp>

namespace tomurcuk {
    template<typename Object>
    class ObjectOwner {
    public:
        static auto of(Object *pointer) -> ObjectOwner<Object> {
            ObjectOwner<Object> objectOwner;
            objectOwner.mPointer = pointer;
            return objectOwner;
        }

        static auto null() -> ObjectOwner<Object> {
            return of(nullptr);
        }

        static auto create(MemoryAllocator memoryAllocator) -> Result<ObjectOwner<Object>> {
            auto blockResult = memoryAllocator.allocate(sizeof(Object), alignof(Object));
            if (blockResult.isFailure()) {
                return Result<ObjectOwner<Object>>::failure();
            }

            return Results::success(of((Object *)*blockResult.value()));
        }

        auto destroy(MemoryAllocator memoryAllocator) -> void {
            memoryAllocator.deallocate(mPointer, sizeof(Object), alignof(Object));
        }

        auto isNull() -> bool {
            return mPointer == nullptr;
        }

        auto get() -> Object * {
            return mPointer;
        }

    private:
        Object *mPointer;
    };
}
