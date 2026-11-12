/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_RANGE_H
#define HX_CORE_RANGE_H

#include "IntTypes.h"
#include "FloatTypes.h"

#ifndef HX_RANGE
#define HX_RANGE

#ifdef __cplusplus
#include <algorithm>

namespace HxLibs
{
	//表示一个区间，通过起点和止点。
	template <typename T>
	struct Range
	{
	private:
		T m_from;
		T m_to;

	public:
		HX_INLINE Range() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC Range(T from, T to) HX_NOEXCEPT
			: m_from(from), m_to(to)
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
			return m_to;
		}
		HX_CONSTEXPR11_FUNC T end() const HX_NOEXCEPT
		{
			return m_to;
		}

		HX_CONSTEXPR11_FUNC T size() const HX_NOEXCEPT
		{
			return m_to - m_from;
		}

		//重置区间。
		HX_INLINE void reset(T from, T to) HX_NOEXCEPT
		{
			m_from = from;
			m_to = to;
		}
		//重置区间。
		HX_INLINE void reset(const Range& other) HX_NOEXCEPT
		{
			m_from = other.m_from;
			m_to = other.m_to;
		}

		//向大延长，起点不变。
		HX_INLINE void extend(T distance) HX_NOEXCEPT
		{
			m_to += distance;
		}
		//向大缩短，止点不变。
		HX_INLINE void shrink(T distance) HX_NOEXCEPT
		{
			m_from += distance;
		}

		//向大延长，起点不变。
		HX_INLINE Range& operator +=(T extent) HX_NOEXCEPT
		{
			m_to += extent;
			return *this;
		}
		//向大延长，起点不变。
		HX_CONSTEXPR11_FUNC Range operator +(T extent) const HX_NOEXCEPT
		{
			return Range(m_from, m_to + extent);
		}

		//向大缩短，止点不变。
		HX_INLINE Range& operator -=(T shrink) HX_NOEXCEPT
		{
			m_from += shrink;
			return *this;
		}
		//向大缩短，止点不变。
		HX_CONSTEXPR11_FUNC Range operator -(T shrink) const HX_NOEXCEPT
		{
			return Range(m_from + shrink, m_to);
		}

		//往大平移。
		HX_CONSTEXPR11_FUNC Range operator >>(T distance) const HX_NOEXCEPT
		{
			return Range(m_from + distance, m_to + distance);
		}
		//往大平移。
		HX_INLINE Range& operator >>=(T distance) HX_NOEXCEPT
		{
			m_from += distance;
			m_to += distance;
			return *this;
		}

		//往小平移。
		HX_CONSTEXPR11_FUNC Range operator <<(T distance) const HX_NOEXCEPT
		{
			return Range(m_from - distance, m_to - distance);
		}
		//往小平移。
		HX_INLINE Range& operator <<=(T distance) HX_NOEXCEPT
		{
			m_from -= distance;
			m_to -= distance;
			return *this;
		}

		//等比放大。
		template <typename TScale>
		HX_CONSTEXPR11_FUNC Range operator *(TScale scale) const HX_NOEXCEPT
		{
			return Range((T)(m_from * scale), (T)(m_to * scale));
		}
		//等比放大。
		template <typename TScale>
		HX_INLINE Range& operator *=(TScale scale) HX_NOEXCEPT
		{
			m_from = (T)(m_from * scale);
			m_to = (T)(m_to * scale);
			return *this;
		}

		//等比缩小。
		template <typename TScale>
		HX_CONSTEXPR11_FUNC Range operator /(TScale scale) const HX_NOEXCEPT
		{
			return Range((T)(m_from / scale), (T)(m_to / scale));
		}
		//等比缩小。
		template <typename TScale>
		HX_INLINE Range& operator /=(TScale scale) HX_NOEXCEPT
		{
			m_from = (T)(m_from / scale);
			m_to = (T)(m_to / scale);
			return *this;
		}

		//是否相接，一前一后.
		HX_CONSTEXPR11_FUNC bool operator ||(const Range& other) const HX_NOEXCEPT
		{
			return m_from <= other.m_from && m_to == other.m_from;
		}

		//相接，一前一后。
		HX_CONSTEXPR11_FUNC Range operator |(const Range& other) const HX_NOEXCEPT
		{
			return Range(m_from, other.m_to);
		}
		//相接，一前一后。
		HX_INLINE Range& operator |=(const Range& other) HX_NOEXCEPT
		{
			m_to = other.m_to;
			return *this;
		}

		//是否相交，不分前后。
		HX_CONSTEXPR11_FUNC bool operator &&(const Range& other) const HX_NOEXCEPT
		{
			return !(m_to <= other.m_from || m_from >= other.m_to);
		}

		//求交，不分前后。
		HX_CONSTEXPR11_FUNC Range operator &(const Range& other) const HX_NOEXCEPT
		{
			return Range(::std::max(m_from, other.m_from), ::std::min(m_to, other.m_to));
		}
		//求交，不分前后。
		HX_INLINE Range& operator &=(const Range& other) HX_NOEXCEPT
		{
			if (m_from < other.m_from)
			{
				m_from = other.m_from;
			}
			if (m_to > other.m_to)
			{
				m_to = other.m_to;
			}
			return *this;
		}

		//求并，不分前后。
		HX_CONSTEXPR11_FUNC Range operator +(const Range& other) const HX_NOEXCEPT
		{
			return Range(::std::min(m_from, other.m_from), ::std::max(m_to, other.m_to));
		}
		//求并，不分前后。
		HX_INLINE Range& operator +=(const Range& other) HX_NOEXCEPT
		{
			if (m_from > other.m_from)
			{
				m_from = other.m_from;
			}
			if (m_to < other.m_to)
			{
				m_to = other.m_to;
			}
			return *this;
		}

		//
		HX_INLINE void subtract(const Range& other) HX_NOEXCEPT
		{
			m_to = other.m_from;
		}
		//
		HX_INLINE void concatenate(const Range& other) HX_NOEXCEPT
		{
			m_to = other.m_to;
		}
		//与指定的区间进行合并。
		HX_INLINE void merge(const Range& other) HX_NOEXCEPT
		{
			m_from = ::std::min(m_from, other.m_from);
			m_to = ::std::max(m_to, other.m_to);
		}

		//计算与指定的区间的交集。
		HX_INLINE void intersect(const Range& other) HX_NOEXCEPT
		{
			m_from = ::std::max(m_from, other.m_from);
			m_to = ::std::min(m_to, other.m_to);
		}
		//计算与指定的区间的交集。
		HX_CONSTEXPR11_FUNC Range intersectWith(const Range& other) const HX_NOEXCEPT
		{
			return { ::std::max(m_from, other.m_from), ::std::min(m_to, other.m_to) };
		}
		//检查是否与指定的区间相交。
		HX_CONSTEXPR11_FUNC bool isIntersectWith(const Range& other) const HX_NOEXCEPT
		{
			return !(m_to <= other.m_from || m_from >= other.m_to);
		}

		//是否包含指定的区间。
		HX_CONSTEXPR11_FUNC bool isContaining(T from, T to) const HX_NOEXCEPT
		{
			return m_from <= from && m_to >= m_to;
		}
		//是否包含指定的区间。
		HX_CONSTEXPR11_FUNC bool isContaining(const Range& other) const HX_NOEXCEPT
		{
			return m_from <= other.m_from && m_to >= other.m_to;
		}
	};
}

template <typename T>
using HxRange = ::HxLibs::Range<T>;

#define HX_RANGE_DEFINE(NAME, TYPE) using NAME = ::HxLibs::Range<TYPE>;

#else //__cplusplus

#define HX_RANGE_DEFINE(NAME, TYPE)\
typedef struct NAME\
{\
	TYPE from;\
	TYPE to;\
} NAME;

#endif //__cplusplus

HX_RANGE_DEFINE(HxRange8u, Hx8u);
HX_RANGE_DEFINE(HxRange8s, Hx8s);
HX_RANGE_DEFINE(HxRange16u, Hx16u);
HX_RANGE_DEFINE(HxRange16s, Hx16s);
HX_RANGE_DEFINE(HxRange32u, Hx32u);
HX_RANGE_DEFINE(HxRange32s, Hx32s);
HX_RANGE_DEFINE(HxRange64u, Hx64u);
HX_RANGE_DEFINE(HxRange64s, Hx64s);
HX_RANGE_DEFINE(HxRange32f, Hx32f);
HX_RANGE_DEFINE(HxRange64f, Hx64f);

#endif //!HX_RANGE

#endif //!HX_CORE_RANGE_H
