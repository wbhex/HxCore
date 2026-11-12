/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_INT_TYPES_H
#define HX_CORE_INT_TYPES_H

#include <stdint.h>
#include <limits.h>
#include "Defines.h"

#ifndef HX_INT_TYPES
#define HX_INT_TYPES

typedef int8_t Hx8s;
typedef uint8_t Hx8u;
typedef int16_t Hx16s;
typedef uint16_t Hx16u;
typedef int32_t Hx32s;
typedef uint32_t Hx32u;
#if HX_WINDOWS
typedef int64_t Hx64s;
typedef uint64_t Hx64u;
#else //!HX_WINDOWS
typedef long long Hx64s;
typedef unsigned long long Hx64u;
#endif //HX_WINDOWS

#ifdef __cplusplus
#include <limits>
#include <cstdint>
#include <type_traits>
namespace HxLibs
{
	template <typename T>
	struct IntUtils
	{
		static HX_CONSTEXPR bool Valid = false;
	};

	template <typename T, bool SIGNED, T MIN_VALUE, T MAX_VALUE, typename WIDER_TYPE>
	struct __IntUtils__
	{
		static HX_CONSTEXPR bool Valid = true;
		static HX_CONSTEXPR bool Signed = SIGNED;
		static HX_CONSTEXPR T MinValue = MIN_VALUE;
		static HX_CONSTEXPR T MaxValue = MAX_VALUE;
		using WiderType = WIDER_TYPE;
	};

	template <>
	struct IntUtils<signed char> : public __IntUtils__<signed char, true, INT8_MIN, INT8_MAX, short> {};
	template <>
	struct IntUtils<unsigned char> : public __IntUtils__<unsigned char, false, 0, UINT8_MAX, unsigned short> {};

	template <>
	struct IntUtils<short> : public __IntUtils__<short, true, SHRT_MIN, SHRT_MAX, int> {};
	template <>
	struct IntUtils<unsigned short> : public __IntUtils__<unsigned short, false, 0, USHRT_MAX, unsigned int> {};

	template <>
	struct IntUtils<int> : public __IntUtils__<int, true, INT_MIN, INT_MAX, long long> {};
	template <>
	struct IntUtils<unsigned int> : public __IntUtils__<unsigned int, false, 0, UINT_MAX, unsigned long long> {};

	template <>
	struct IntUtils<long> : public __IntUtils__<long, true, LONG_MIN, LONG_MAX, long long> {};
	template <>
	struct IntUtils<unsigned long> : public __IntUtils__<unsigned long, false, 0, ULONG_MAX, unsigned long long> {};

	template <>
	struct IntUtils<long long> : public __IntUtils__<long long, true, LLONG_MIN, LLONG_MAX, long long> {};
	template <>
	struct IntUtils<unsigned long long> : public __IntUtils__<unsigned long long, false, 0, ULLONG_MAX, unsigned long long> {};
}
#endif //__cplusplus

#endif //!HX_INT_TYPES

#endif //!HX_CORE_INT_TYPES_H
