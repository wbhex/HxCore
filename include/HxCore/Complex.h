/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_COMPLEX_H
#define HX_CORE_COMPLEX_H

#include "IntTypes.h"
#include "FloatTypes.h"

#ifndef HX_COMPLEX
#define HX_COMPLEX

#ifdef __cplusplus

namespace HxLibs
{
	template <typename T>
	struct Complex
	{
		HX_STATIC_ASSERT(::std::is_arithmetic<T>::value, "HxLibs::Complex<T> Type parameter must be arithmetic.");

		union
		{
			T re;
			T i;
		};
		union
		{
			T im;
			T q;
		};

		HX_CONSTEXPR11_FUNC Complex() HX_NOEXCEPT
		{
			/*NOTHING*/
		}

		HX_CONSTEXPR11_FUNC Complex(T re, T im) HX_NOEXCEPT
			: re(re), im(im)
		{
			/*NOTHING*/
		}


		HX_INLINE Complex& operator+=(const Complex& other) HX_NOEXCEPT
		{
			re += other.re;
			im += other.im;
			return *this;
		}

		HX_INLINE Complex& operator-=(const Complex& other) HX_NOEXCEPT
		{
			re -= other.re;
			im -= other.im;
			return *this;
		}

		HX_INLINE Complex& operator*=(const T& scale) HX_NOEXCEPT
		{
			re *= scale;
			im *= scale;
			return *this;
		}

		HX_INLINE Complex& operator/=(const T& scale) HX_NOEXCEPT
		{
			re /= scale;
			im /= scale;
			return *this;
		}

		template <typename TScale>
		HX_INLINE Complex& operator*=(const TScale& scale) HX_NOEXCEPT
		{
			re = (T)(re * scale);
			im = (T)(re * scale);
			return *this;
		}

		template <typename TScale>
		HX_INLINE Complex& operator/=(const TScale& scale) HX_NOEXCEPT
		{
			re = (T)(re / scale);
			im = (T)(re / scale);
			return *this;
		}

		HX_INLINE Complex& operator*=(const Complex& other) HX_NOEXCEPT
		{
			auto reNew = re * other.re - im * other.im;
			auto imNew = re * other.im + im * other.re;
			re = reNew; im = imNew;
			return *this;
		}

		HX_CONSTEXPR11_FUNC Complex operator+(const Complex& other) const HX_NOEXCEPT
		{
			return Complex(re + other.re, im + other.im);
		}

		HX_CONSTEXPR11_FUNC Complex operator-(const Complex& other) const HX_NOEXCEPT
		{
			return Complex(re - other.re, im - other.im);
		}

		HX_CONSTEXPR11_FUNC Complex operator*(T scale) const HX_NOEXCEPT
		{
			return Complex(re * scale, im * scale);
		}

		HX_CONSTEXPR11_FUNC Complex operator/(T scale) const HX_NOEXCEPT
		{
			return Complex(re / scale, im / scale);
		}

		template <typename TScale>
		HX_CONSTEXPR11_FUNC Complex operator*(TScale scale) const HX_NOEXCEPT
		{
			return Complex((T)(re * scale), (T)(im * scale));
		}

		template <typename TScale>
		HX_CONSTEXPR11_FUNC Complex operator/(TScale scale) const HX_NOEXCEPT
		{
			return Complex((T)(re / scale), (T)(im / scale));
		}

		HX_CONSTEXPR11_FUNC Complex operator*(const Complex& other) const HX_NOEXCEPT
		{
			return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
		}
	};
}

template <typename T>
using HxComplex = ::HxLibs::Complex<T>;

#define HX_COMPLEX_DEFINE(NAME, TYPE) using NAME = ::HxLibs::Complex<TYPE>;

#else //__cplusplus

#define HX_COMPLEX_DEFINE(NAME, TYPE)\
typedef struct\
{\
	union\
	{\
		TYPE re;\
		TYPE i;\
	};\
	union\
	{\
		TYPE im;\
		TYPE q;\
	};\
} NAME;

#endif //__cplusplus

HX_COMPLEX_DEFINE(Hx8uc, Hx8u);
HX_COMPLEX_DEFINE(Hx8sc, Hx8s);
HX_COMPLEX_DEFINE(Hx16uc, Hx16u);
HX_COMPLEX_DEFINE(Hx16sc, Hx16s);
HX_COMPLEX_DEFINE(Hx32uc, Hx32u);
HX_COMPLEX_DEFINE(Hx32sc, Hx32s);
HX_COMPLEX_DEFINE(Hx64uc, Hx64u);
HX_COMPLEX_DEFINE(Hx64sc, Hx64s);
HX_COMPLEX_DEFINE(Hx32fc, Hx32f);
HX_COMPLEX_DEFINE(Hx64fc, Hx64f);

#endif //!HX_COMPLEX

#include "BufView.h"

#ifndef HX_COMPLEXES
#define HX_COMPLEXES

HX_BUFVIEW_DEFINE(Hx8sc)
HX_BUFVIEW_DEFINE(Hx8uc)
HX_BUFVIEW_DEFINE(Hx16sc)
HX_BUFVIEW_DEFINE(Hx16uc)
HX_BUFVIEW_DEFINE(Hx32sc)
HX_BUFVIEW_DEFINE(Hx32uc)
HX_BUFVIEW_DEFINE(Hx64sc)
HX_BUFVIEW_DEFINE(Hx64uc)
HX_BUFVIEW_DEFINE(Hx32fc)
HX_BUFVIEW_DEFINE(Hx64fc)

#endif //!HX_COMPLEXES 

#endif // !HX_CORE_COMPLEX_H
