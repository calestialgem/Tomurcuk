#pragma once

#include <assert.h>
#include <stdint.h>
#include <tomurcuk/ArrayReference.hpp>

namespace tomurcuk {
    template<typename Element>
    class ArrayView {
    public:
        static auto of(ArrayReference<Element> arrayReference, int64_t length) -> ArrayView {
            ArrayView arrayView;
            arrayView.mArrayReference = arrayReference;
            arrayView.mLength = length;
            return arrayView;
        }

        static auto null() -> ArrayView {
            return of(ArrayReference<Element>::null(), 0);
        }

        auto isNull() -> bool {
            return mArrayReference.isNull();
        }

        auto reference() -> ArrayReference<Element> {
            return mArrayReference;
        }

        auto pointer() -> Element * {
            return mArrayReference.pointer();
        }

        auto first() -> Element * {
            return get(0);
        }

        auto last() -> Element * {
            return get(mLength - 1);
        }

        auto get(int64_t index) -> Element * {
            assert(index < mLength);

            return mArrayReference.get(index);
        }

    private:
        ArrayReference<Element> mArrayReference;
        int64_t mLength;
    };
}
