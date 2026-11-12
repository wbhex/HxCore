/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_ENUM_H
#define HX_CORE_ENUM_H

#include "Defines.h"

#ifndef HX_ENUM_H
#define HX_ENUM_H

#ifdef __cplusplus
#	define HX_ENUM_DECLARE(NAME) enum NAME
#	define HX_ENUM_DECLARE_BEGIN(NAME) enum NAME
#	define HX_ENUM_DECLARE_END(NAME)
#	define HX_ENUM_ALIAS(TSRC,TDST) using TDST = TSRC
#	ifndef HX_FLAG_ENUM_MAKE
#		include <type_traits>
#		define HX_FLAG_ENUM_MAKE(T) \
		HX_INLINE const T operator & (T a, T b) HX_NOEXCEPT { static_assert(std::is_enum<T>::value, "HX_FLAG_ENUM_MAKE(T) Type T must be a enum."); using UT = std::underlying_type<T>::type; return (T)((UT)a & (UT)b); } \
		HX_INLINE T& operator &= (T& a, T b) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return reinterpret_cast<T&>(reinterpret_cast<UT&>(a) &= (UT)b); } \
		HX_INLINE const T operator | (T a, T b) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return (T)((UT)a | (UT)b); } \
		HX_INLINE T& operator |= (T& a, T b) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return reinterpret_cast<T&>(reinterpret_cast<UT&>(a) |= (UT)b); } \
		HX_INLINE const T operator ^ (T a, T b) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return (T)((UT)a ^ (UT)b); } \
		HX_INLINE T& operator ^= (T& a, T b) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return reinterpret_cast<T&>(reinterpret_cast<UT&>(a) ^= (UT)b); } \
		HX_INLINE const T operator ~ (T a) HX_NOEXCEPT { using UT = std::underlying_type<T>::type; return (T)~(UT)a; }
#	endif //!HX_FLAG_ENUM_MAKE
#else //__cplusplus
#	define HX_ENUM_DECLARE(NAME) typedef enum NAME NAME; enum NAME
#	define HX_ENUM_DECLARE_BEGIN(NAME) enum NAME
#	define HX_ENUM_DECLARE_END(NAME) typedef enum NAME NAME;
#	define HX_ENUM_ALIAS(TSRC,TDST) typedef TSRC TDST
#endif //__cplusplus

#endif //!HX_ENUM_H

#endif //!HX_CORE_ENUM_H
