/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_GCD_FUNC_H
#define HX_CORE_GCD_FUNC_H

#ifdef __cplusplus
#ifndef HX_GCD_CPPFUNC

#include "Defines.h"
#include <type_traits>

namespace HxLibs
{
	//执行GCD计算处理
	template <typename T>
	HX_CONSTEXPR17_FUNC static T __gcd__(T a, T b) HX_NOEXCEPT
	{
		HX_STATIC_ASSERT(::std::is_integral<T>::value, "HxLibs::__gcd__<T>() Type parameter must be integral.");
		while (b != 0)
		{
			const T a2 = a;
			a = b;
			b = a2 % b;
		}
		return a;
	}

	//计算两个数的最大公约数
	template <typename T, bool = ::std::is_signed<T>::value>
	struct __GcdFunc__;
	//计算两个数的最大公约数，有符号整数
	template <typename T>
	struct __GcdFunc__<T, true>
	{
		HX_CONSTEXPR17_FUNC static T exec(T a, T b) HX_NOEXCEPT
		{
			if (a == 0 && b == 0)
			{
				return 1;
			}
			return __gcd__(a < 0 ? -a : a, b < 0 ? -b : b);
		}
	};
	//计算两个数的最大公约数，无符号整数
	template <typename T>
	struct __GcdFunc__<T, false>
	{
		HX_CONSTEXPR17_FUNC static T exec(T a, T b) HX_NOEXCEPT
		{
			if (a == 0 && b == 0)
			{
				return 1;
			}
			return __gcd__(a, b);
		}
	};

	//计算两个数的最大公约数
	template <typename T>
	struct GcdFunc : public __GcdFunc__<T, ::std::is_signed<T>::value> {};
	//计算两个数的最大公约数
	template <typename T>
	HX_CONSTEXPR17_FUNC static T gcd(T a, T b) HX_NOEXCEPT
	{
		return GcdFunc<T>::exec(a, b);
	}
}
#endif //HX_GCD_CPPFUNC
#endif //__cplusplus

#endif //!HX_CORE_GCD_FUNC_H
