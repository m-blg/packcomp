#pragma once

#include "mbgldef.h"
#include <cassert>

namespace cp {
    
    template <typename T>
    inline T min(T first, T second) {
        return ((first < second) ? first : second);
    }

    template <typename T>
    inline T max(T first, T second) {
        return ((first > second) ? first : second);
    }

    template <typename T>
    bool
    is_bounded(T value, T min_value, T max_value) {
        return (min_value <= value && value <= max_value);
    }


    template <typename T>
    inline void swap(T *first, T *second) {
        T temp = *first;
        *first = *second;
        *second = temp;
    }

}
