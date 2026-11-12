/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SPAN_H
#define HX_CORE_SPAN_H

#include "IntTypes.h"
#include "FloatTypes.h"

#ifndef HX_SPAN
#define HX_SPAN

#ifdef __cplusplus
#include <algorithm>

namespace HxLibs
{
	//表示一个区间，通过起点和宽度。
	template <typename T>
	struct Span
	{
	private:
		T m_from;
		T m_size;

	public:
		HX_INLINE Span()
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC Span(T from, T size)
			: m_from(from), m_size(size)
		{
			//NOTHING
		}

		HX_CONSTEXPR11_FUNC T from() const HX_NOEXCEPT
		{
			return m_from;
		}
		HX_CONSTEXPR11_FUNC T begin() const HX_NOEXCEPT
		{
			return m_from;
		}

		HX_CONSTEXPR11_FUNC T to() const HX_NOEXCEPT
		{
			return m_from + m_size;
		}
		HX_CONSTEXPR11_FUNC T end() const HX_NOEXCEPT
		{
			return m_from + m_size;
		}

		HX_CONSTEXPR11_FUNC T size() const HX_NOEXCEPT
		{
			return m_size;
		}

		//重置区间。
		HX_INLINE void reset(T from, T size) HX_NOEXCEPT
		{
			m_from = from;
			m_size = size;
		}
		//重置区间。
		HX_INLINE void reset(const Span& other) HX_NOEXCEPT
		{
			m_from = other.m_from;
			m_size = other.m_size;
		}

		//向大延长，起点不变。
		HX_INLINE void extend(T distance) HX_NOEXCEPT
		{
			m_size += distance;
		}
		//向大缩短，止点不变。
		HX_INLINE void shrink(T distance) HX_NOEXCEPT
		{
			m_from += distance;
		}

		//向大延长，起点不变。
		HX_INLINE Span& operator +=(T extent) HX_NOEXCEPT
		{
			m_size += extent;
			return *this;
		}
		//向大延长，起点不变。
		HX_CONSTEXPR11_FUNC Span operator +(T extent) const HX_NOEXCEPT
		{
			return Span(m_from, m_size + extent);
		}

		//向大缩短，止点不变。
		HX_INLINE Span& operator -=(T shrink) HX_NOEXCEPT
		{
			m_from += shrink;
			return *this;
		}
		//向大缩短，止点不变。
		HX_CONSTEXPR11_FUNC Span operator -(T shrink) const HX_NOEXCEPT
		{
			return Span(m_from + shrink, m_size);
		}

		//向大平移，起点止点同时变大，长度不变。
		HX_INLINE Span& operator >>=(T distance) HX_NOEXCEPT
		{
			m_from += distance;
			return *this;
		}
		//向大平移，起点止点同时变大，长度不变。
		HX_CONSTEXPR11_FUNC Span operator >>(T distance) const HX_NOEXCEPT
		{
			return Span(m_from + distance, m_size);
		}

		//向小平移，起点止点同时变小，长度不变。
		HX_INLINE Span& operator <<=(T distance) HX_NOEXCEPT
		{
			m_from -= distance;
			return *this;
		}
		//向小平移，起点止点同时变小，长度不变。
		HX_CONSTEXPR11_FUNC Span operator <<(T distance) const HX_NOEXCEPT
		{
			return Span(m_from - distance, m_size);
		}

		//向大比例扩张，起点不变。
		template <typename TScale>
		HX_INLINE Span& operator *=(TScale scale) HX_NOEXCEPT
		{
			m_size = (T)(m_size * scale);
			return *this;
		}
		//向大比例扩张，起点不变。
		template <typename TScale>
		HX_CONSTEXPR11_FUNC Span operator *(TScale scaleUp) const HX_NOEXCEPT
		{
			return Span(m_from, (T)(m_size * scaleUp));
		}

		//向大比例缩短，止点不变。
		template <typename TScale>
		HX_INLINE Span& operator /=(TScale scale) HX_NOEXCEPT
		{
			m_from += (T)(m_size / scale);
			return *this;
		}
		//向大比例缩短，止点不变。
		template <typename TScale>
		HX_CONSTEXPR11_FUNC Span operator /(TScale scaleDown) const HX_NOEXCEPT
		{
			return Span(m_from + (T)(m_size / scaleDown), m_size);
		}

		//返回从开始计算的子区间
		HX_CONSTEXPR11_FUNC Span subFront(T size, T offset = 0) const HX_NOEXCEPT
		{
			return Span(m_from + offset, size);
		}
		//返回从结束计算的子区间
		HX_CONSTEXPR11_FUNC Span subEnd(T size, T offset = 0) const HX_NOEXCEPT
		{
			return Span(to() - size - offset, size);
		}

		//检查是否与指定的区间相交。
		HX_CONSTEXPR11_FUNC bool checkIntersect(const Span& b) const HX_NOEXCEPT
		{
			return m_from < b.to() && to() > b.m_from;
		}
		//返回与指定区间的相交区间。
		HX_CONSTEXPR11_FUNC Span intersect(const Span& b) const HX_NOEXCEPT
		{
			return (m_from >= b.to() || to() <= b.m_from)
				? Span{ m_from, (T)0 }
				: (m_from <= b.m_from
					? Span{ b.m_from, to() - b.m_from }
			: Span{ m_from, b.to() - m_from });
		}
	};
}

template <typename T>
using HxSpan = ::HxLibs::Span<T>;

#define HX_SPAN_DEFINE(NAME, TYPE) using NAME = ::HxLibs::Span<TYPE>;

#else //__cplusplus

#define HX_SPAN_DEFINE(NAME, TYPE)\
typedef struct NAME\
{\
	TYPE from;\
	TYPE size;\
} NAME;

#endif //__cplusplus

HX_SPAN_DEFINE(HxSpan8u, Hx8u);
HX_SPAN_DEFINE(HxSpan8s, Hx8s);
HX_SPAN_DEFINE(HxSpan16u, Hx16u);
HX_SPAN_DEFINE(HxSpan16s, Hx16s);
HX_SPAN_DEFINE(HxSpan32u, Hx32u);
HX_SPAN_DEFINE(HxSpan32s, Hx32s);
HX_SPAN_DEFINE(HxSpan64u, Hx64u);
HX_SPAN_DEFINE(HxSpan64s, Hx64s);
HX_SPAN_DEFINE(HxSpan32f, Hx32f);
HX_SPAN_DEFINE(HxSpan64f, Hx64f);

#endif //!HX_SPAN

#endif //!HX_CORE_SPAN_H
