#pragma once
#ifndef HX_CORE_HASH_UTILS_H
#define HX_CORE_HASH_UTILS_H

#include "Defines.h"

HX_CONSTEXPR17_FUNC size_t HxHash_fnv1a(const unsigned char* first, size_t count) HX_NOEXCEPT
{
#if HX_64BIT
    HX_STATIC_ASSERT((sizeof(size_t) == 8), "This code is for 64-bit size_t.");
    const size_t fnv_offset_basis = 14695981039346656037ULL;
    const size_t fnv_prime = 1099511628211ULL;
#else //!HX_64BIT
    HX_STATIC_ASSERT((sizeof(size_t) == 4), "This code is for 32-bit size_t.");
    const size_t fnv_offset_basis = 2166136261U;
    const size_t fnv_prime = 16777619U;
#endif //!HX_64BIT

    size_t result = fnv_offset_basis;
    for (size_t next = 0; next < count; ++next)
    {
        // fold in another byte
        result ^= (size_t)first[next];
        result *= fnv_prime;
    }
    return (result);
}

#endif //!HX_CORE_HASH_UTILS_H
