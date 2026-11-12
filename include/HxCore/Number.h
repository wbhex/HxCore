/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_NUMBER_H
#define HX_CORE_NUMBER_H

#include "IntTypes.h"
#include "FloatTypes.h"

#ifndef HX_NUMBER
#define HX_NUMBER

#ifdef __cplusplus

namespace HxLibs
{
	template <typename T>
	class Number
	{
		HX_STATIC_ASSERT(::std::is_arithmetic<T>::value, "HxLibs::Number<T> Type parameter must be arithmetic.");

	protected:
		T m_v;

	public:
		HX_INLINE Number() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC Number(T v) HX_NOEXCEPT
			: m_v(v)
		{
			//NOTHONG
		}

		HX_INLINE operator T& () HX_NOEXCEPT
		{
			return m_v;
		}
		HX_CONSTEXPR11_FUNC operator const T& () const HX_NOEXCEPT
		{
			return m_v;
		}
	};
}

template <typename T>
using HxNumber = ::HxLibs::Number<T>;

#define HX_NUMBER_DEFINE(NAME, TYPE) using NAME = ::HxLibs::Number<TYPE>;

#else //__cplusplus

#define HX_NUMBER_DEFINE(NAME, TYPE) typedef TYPE NAME;

#endif //__cplusplus

HX_NUMBER_DEFINE(HxInt8, Hx8s)
HX_NUMBER_DEFINE(HxUInt8, Hx8u)
HX_NUMBER_DEFINE(HxInt16, Hx16s)
HX_NUMBER_DEFINE(HxUInt16, Hx16u)
HX_NUMBER_DEFINE(HxInt32, Hx32s)
HX_NUMBER_DEFINE(HxUInt32, Hx32u)
HX_NUMBER_DEFINE(HxInt64, Hx64s)
HX_NUMBER_DEFINE(HxUInt64, Hx64u)
HX_NUMBER_DEFINE(HxFloat32, Hx32f)
HX_NUMBER_DEFINE(HxFloat64, Hx64f)

#endif //!HX_NUMBER

#include "BufView.h"

#ifndef HX_NUMBERS
#define HX_NUMBERS

HX_BUFVIEW_DEFINE(Hx8s)
HX_BUFVIEW_DEFINE(Hx8u)
HX_BUFVIEW_DEFINE(Hx16s)
HX_BUFVIEW_DEFINE(Hx16u)
HX_BUFVIEW_DEFINE(Hx32s)
HX_BUFVIEW_DEFINE(Hx32u)
HX_BUFVIEW_DEFINE(Hx64s)
HX_BUFVIEW_DEFINE(Hx64u)
HX_BUFVIEW_DEFINE(Hx32f)
HX_BUFVIEW_DEFINE(Hx64f)

#endif //!HX_NUMBERS

#endif //!HX_CORE_NUMBER_H
