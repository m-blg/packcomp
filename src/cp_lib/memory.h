
#pragma once
#include "mbgldef.h"
#include <stdlib.h>
#include <stdio.h>

namespace cp 
{

template <typename T>
inline T* m_alloc(size_t count) {
    return (T*)malloc(count * sizeof(T));
}

template <typename T>
inline T* m_nalloc(size_t count) {
    return (T*)calloc(count, sizeof(T));
}

template <typename T>
inline T* m_ralloc(T* ptr, size_t count) {
    return (T*)realloc(ptr, count * sizeof(T));
}

}
