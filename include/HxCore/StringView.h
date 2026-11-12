/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRING_VIEW_H
#define HX_CORE_STRING_VIEW_H

#ifndef HX_STRING_VIEW
#define HX_STRING_VIEW

#include "Char.h"

#ifdef __cplusplus

#include "String.h"
#include "ArrayView.h"
#include "HashUtils.h"
#include "CStringLength.h"

namespace HxLibs
{
	template <typename Char = char>
	struct XStringView : public ArrayConstView<Char>
	{
		using _MyBase = ArrayConstView<Char>;

	public:
		//构造函数。
		HX_CONSTEXPR11_FUNC XStringView() HX_NOEXCEPT
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC XStringView(::std::nullptr_t) HX_NOEXCEPT
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC XStringView(const Char* v) HX_NOEXCEPT
			: _MyBase(v, CStringLength::count(v))
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC XStringView(const Char* v, size_t l) HX_NOEXCEPT
			: _MyBase(v, l)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC XStringView(const Char* begin, const Char* end) HX_NOEXCEPT
			: _MyBase(begin, end)
		{
			//NOTHING
		}
		//构造函数。
		HX_INLINE XStringView(const Sequence<Char*>& v) HX_NOEXCEPT
			: _MyBase(v.ptr(), v.size())
		{
			//NOTHING
		}
		//构造函数。
		HX_INLINE XStringView(const Sequence<const Char*>& v) HX_NOEXCEPT
			: _MyBase(v)
		{
			//NOTHING
		}
		//构造函数。
		template <typename Traits, typename Alloc>
		HX_INLINE XStringView(const XString<Char, Traits, Alloc>& v) HX_NOEXCEPT
			: _MyBase(v.c_str(), v.size())
		{
			//NOTHONG
		}
#if _HAS_CXX17
		//构造函数。
		template <typename Traits>
		HX_INLINE XStringView(const ::std::basic_string_view<Char, Traits>& v) HX_NOEXCEPT
			: _MyBase(v.data(), v.size())
		{
			//NOTHONG
		}
#endif //_HAS_CXX17

		//截取相应内容
		HX_INLINE XStringView extract(size_t pos) const HX_NOEXCEPT
		{
			return XStringView(_MyBase::m_ptr + pos, _MyBase::m_size - pos);
		}
		//截取相应内容
		HX_INLINE XStringView extract(size_t pos, size_t cnt) const HX_NOEXCEPT
		{
			return XStringView(_MyBase::m_ptr + pos, cnt);
		}

		//转换为字符串。
		template <typename Traits = CharDefaultTraits<Char>, typename Alloc = StringDefaultAllocator<Char>>
		HX_INLINE XString<Char, Traits, Alloc> toString() const HX_NOEXCEPT
		{
			return XString<Char, Traits, Alloc>(_MyBase::m_ptr, _MyBase::m_size);
		}
		//显示转换操作
		template <typename Traits, typename Alloc>
		HX_INLINE explicit operator XString<Char, Traits, Alloc>() const HX_NOEXCEPT
		{
			return XString<Char, Traits, Alloc>(_MyBase::m_ptr, _MyBase::m_size);
		}
#if _HAS_CXX17
		//隐式转换操作
		template <typename Traits>
		HX_INLINE operator ::std::basic_string_view<Char, Traits>() const HX_NOEXCEPT
		{
			return ::std::basic_string_view<Char, Traits>(_MyBase::m_ptr, _MyBase::m_size);
		}
#endif //_HAS_CXX17

		//执行比较操作
		HX_INLINE int compare(const Char* other) const HX_NOEXCEPT
		{
			if (_MyBase::m_size == 0)
			{//当前为空
				if (other == nullptr || *other == 0)
				{//都为空，则表示相同
					return 0;
				}
				//空字符串始终小于不为空字符串
				return -1;
			}
			if (other != nullptr)
			{//内容比较
				if (_MyBase::m_ptr == other)
				{//指向相同，则判断末尾是否为0
					return other[_MyBase::m_size] == 0 ? 0 : -1;
				}
				return CharDefaultTraits<Char>::compare(_MyBase::m_ptr, other, _MyBase::m_size);
			}
			//不为空字符串始终大于为空字符串
			return 1;
		}
		//执行比较操作
		HX_INLINE int compare(const Char* other, size_t count) const HX_NOEXCEPT
		{
			if (_MyBase::m_ptr == other)
			{//指向相同，则比较长度
				if (_MyBase::m_size == count)
				{
					return 0;
				}
				else if (_MyBase::m_size < count)
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
			if (_MyBase::m_size == 0)
			{//当前为空
				if (count == 0)
				{//都为空，则表示相同
					return 0;
				}
				//空字符串始终小于不为空字符串
				return -1;
			}
			if (count > 0)
			{//内容比较
				return CharDefaultTraits<Char>::compare(_MyBase::m_ptr, other, _MyBase::m_size > count ? _MyBase::m_size : count);
			}
			//不为空字符串始终大于为空字符串
			return 1;
		}
		//执行比较操作
		HX_INLINE int compare(const XStringView& other) const HX_NOEXCEPT
		{
			return compare(other.ptr(), other.size());
		}
		//执行比较操作
		HX_INLINE int compare(const Sequence<Char*>& other) const HX_NOEXCEPT
		{
			return compare(other.ptr(), other.size());
		}
		//执行比较操作
		HX_INLINE int compare(const Sequence<const Char*>& other) const HX_NOEXCEPT
		{
			return compare(other.ptr(), other.size());
		}
		//执行比较操作
		template <typename Traits, typename Alloc>
		HX_INLINE int compare(const XString<Char, Traits, Alloc>& other) const HX_NOEXCEPT
		{
			return compare(other.c_str(), other.size());
		}
#if _HAS_CXX17
		//执行比较操作
		template <typename Traits>
		HX_INLINE int compare(const ::std::basic_string_view<Char, Traits>& other) const HX_NOEXCEPT
		{
			return compare(other.data(), other.size());
		}
#endif //_HAS_CXX17

		//判断是否相等
		HX_INLINE bool equals(const Char* other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//判断是否相等
		HX_INLINE bool equals(const XStringView& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//判断是否相等
		HX_INLINE bool equals(const Sequence<Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//判断是否相等
		HX_INLINE bool equals(const Sequence<const Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//判断是否相等
		template <typename Traits, typename Alloc>
		HX_INLINE bool equals(const XString<Char, Traits, Alloc>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
#if _HAS_CXX17
		//执行比较操作
		template <typename Traits>
		HX_INLINE bool equals(const ::std::basic_string_view<Char, Traits>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
#endif //_HAS_CXX17

		//OPERATOR ==
		HX_INLINE bool operator==(const Char* other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//OPERATOR ==
		HX_INLINE bool operator==(const XStringView& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//OPERATOR ==
		HX_INLINE bool operator==(const Sequence<Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//OPERATOR ==
		HX_INLINE bool operator==(const Sequence<const Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
		//OPERATOR ==
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator==(const XString<Char, Traits, Alloc>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
#if _HAS_CXX17
		//OPERATOR ==
		template <typename Traits>
		HX_INLINE bool operator==(const ::std::basic_string_view<Char, Traits>& other) const HX_NOEXCEPT
		{
			return compare(other) == 0;
		}
#endif //_HAS_CXX17

		//OPERATOR !=
		HX_INLINE bool operator!=(const Char* other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
		//OPERATOR !=
		HX_INLINE bool operator!=(const XStringView& other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
		//OPERATOR !=
		HX_INLINE bool operator!=(const Sequence<Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
		//OPERATOR !=
		HX_INLINE bool operator!=(const Sequence<const Char*>& other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
		//OPERATOR !=
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator!=(const XString<Char, Traits, Alloc>& other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
#if _HAS_CXX17
		//OPERATOR !=
		template <typename Traits>
		HX_INLINE bool operator!=(const ::std::basic_string_view<Char, Traits>& other) const HX_NOEXCEPT
		{
			return compare(other) != 0;
		}
#endif //_HAS_CXX17

		//OPERATOR <
		HX_INLINE bool operator<(const Char* right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
		//OPERATOR <
		HX_INLINE bool operator<(const XStringView& right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
		//OPERATOR <
		HX_INLINE bool operator<(const Sequence<Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
		//OPERATOR <
		HX_INLINE bool operator<(const Sequence<const Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
		//OPERATOR <
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator<(const XString<Char, Traits, Alloc>& right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
#if _HAS_CXX17
		//OPERATOR <
		template <typename Traits>
		HX_INLINE bool operator<(const ::std::basic_string_view<Char, Traits>& right) const HX_NOEXCEPT
		{
			return compare(right) < 0;
		}
#endif //_HAS_CXX17

		//OPERATOR <=
		HX_INLINE bool operator<=(const Char* right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
		//OPERATOR <=
		HX_INLINE bool operator<=(const XStringView& right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
		//OPERATOR <=
		HX_INLINE bool operator<=(const Sequence<Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
		//OPERATOR <=
		HX_INLINE bool operator<=(const Sequence<const Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
		//OPERATOR <=
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator<=(const XString<Char, Traits, Alloc>& right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
#if _HAS_CXX17
		//OPERATOR <=
		template <typename Traits>
		HX_INLINE bool operator<=(const ::std::basic_string_view<Char, Traits>& right) const HX_NOEXCEPT
		{
			return compare(right) <= 0;
		}
#endif //_HAS_CXX17

		//OPERATOR >
		HX_INLINE bool operator>(const Char* right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
		//OPERATOR >
		HX_INLINE bool operator>(const XStringView& right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
		//OPERATOR >
		HX_INLINE bool operator>(const Sequence<Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
		//OPERATOR >
		HX_INLINE bool operator>(const Sequence<const Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
		//OPERATOR >
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator>(const XString<Char, Traits, Alloc>& right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
#if _HAS_CXX17
		//OPERATOR >
		template <typename Traits>
		HX_INLINE bool operator>(const ::std::basic_string_view<Char, Traits>& right) const HX_NOEXCEPT
		{
			return compare(right) > 0;
		}
#endif //_HAS_CXX17

		//OPERATOR >=
		HX_INLINE bool operator>=(const Char* right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
		//OPERATOR >=
		HX_INLINE bool operator>=(const XStringView& right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
		//OPERATOR >=
		HX_INLINE bool operator>=(const Sequence<Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
		//OPERATOR >=
		HX_INLINE bool operator>=(const Sequence<const Char*>& right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
		//OPERATOR >=
		template <typename Traits, typename Alloc>
		HX_INLINE bool operator>=(const XString<Char, Traits, Alloc>& right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
#if _HAS_CXX17
		//OPERATOR >=
		template <typename Traits>
		HX_INLINE bool operator>=(const ::std::basic_string_view<Char, Traits>& right) const HX_NOEXCEPT
		{
			return compare(right) >= 0;
		}
#endif //_HAS_CXX17
	};

	//OPERATOR ==
	template <typename Char>
	HX_INLINE bool operator==(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) == 0;
	}
	//OPERATOR ==
	template <typename Char>
	HX_INLINE bool operator==(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) == 0;
	}
	//OPERATOR ==
	template <typename Char>
	HX_INLINE bool operator==(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) == 0;
	}
	//OPERATOR ==
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator==(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) == 0;
	}
#if _HAS_CXX17
	//OPERATOR ==
	template <typename Char, typename Traits>
	HX_INLINE bool operator==(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) == 0;
	}
#endif //_HAS_CXX17

	//OPERATOR !=
	template <typename Char>
	HX_INLINE bool operator!=(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) != 0;
	}
	//OPERATOR !=
	template <typename Char>
	HX_INLINE bool operator!=(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) != 0;
	}
	//OPERATOR !=
	template <typename Char>
	HX_INLINE bool operator!=(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) != 0;
	}
	//OPERATOR !=
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator!=(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) != 0;
	}
#if _HAS_CXX17
	//OPERATOR !=
	template <typename Char, typename Traits>
	HX_INLINE bool operator!=(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) != 0;
	}
#endif //_HAS_CXX17

	//OPERATOR <
	template <typename Char>
	HX_INLINE bool operator<(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) > 0;
	}
	//OPERATOR <
	template <typename Char>
	HX_INLINE bool operator<(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) > 0;
	}
	//OPERATOR <
	template <typename Char>
	HX_INLINE bool operator<(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) > 0;
	}
	//OPERATOR <
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator<(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) > 0;
	}
#if _HAS_CXX17
	//OPERATOR <
	template <typename Char, typename Traits>
	HX_INLINE bool operator<(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) > 0;
	}
#endif //_HAS_CXX17

	//OPERATOR <=
	template <typename Char>
	HX_INLINE bool operator<=(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) >= 0;
	}
	//OPERATOR <=
	template <typename Char>
	HX_INLINE bool operator<=(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) >= 0;
	}
	//OPERATOR <=
	template <typename Char>
	HX_INLINE bool operator<=(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) >= 0;
	}
	//OPERATOR <=
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator<=(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) >= 0;
	}
#if _HAS_CXX17
	//OPERATOR <=
	template <typename Char, typename Traits>
	HX_INLINE bool operator<=(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) >= 0;
	}
#endif //_HAS_CXX17

	//OPERATOR >
	template <typename Char>
	HX_INLINE bool operator>(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) < 0;
	}
	//OPERATOR >
	template <typename Char>
	HX_INLINE bool operator>(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) < 0;
	}
	//OPERATOR >
	template <typename Char>
	HX_INLINE bool operator>(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) < 0;
	}
	//OPERATOR >
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator>(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) < 0;
	}
#if _HAS_CXX17
	//OPERATOR >
	template <typename Char, typename Traits>
	HX_INLINE bool operator>(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) < 0;
	}
#endif //_HAS_CXX17

	//OPERATOR >=
	template <typename Char>
	HX_INLINE bool operator>=(const Char* v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) <= 0;
	}
	//OPERATOR >=
	template <typename Char>
	HX_INLINE bool operator>=(const Sequence<Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) <= 0;
	}
	//OPERATOR >=
	template <typename Char>
	HX_INLINE bool operator>=(const Sequence<const Char*>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) <= 0;
	}
	//OPERATOR >=
	template <typename Char, typename Traits, typename Alloc>
	HX_INLINE bool operator>=(const XString<Char, Traits, Alloc>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) <= 0;
	}
#if _HAS_CXX17
	//OPERATOR >=
	template <typename Char, typename Traits>
	HX_INLINE bool operator>=(const ::std::basic_string_view<Char, Traits>& v1, const XStringView<Char>& v2) HX_NOEXCEPT
	{
		return v2.compare(v1) <= 0;
	}
#endif //_HAS_CXX17
	
	using StringView = XStringView<char>;
	using WStringView = XStringView<wchar_t>;
}

namespace std
{
	//定义标准库哈希
	template <typename Char>
	struct hash<::HxLibs::XStringView<Char>>
	{
		using result_type = size_t;
		using argument_type = ::HxLibs::XStringView<Char>;

		HX_CONSTEXPR17_FUNC size_t operator()(const ::HxLibs::XStringView<Char>& v) const HX_NOEXCEPT
		{
			if (v.size() > 0)
			{
				return ::HxHash_fnv1a((const unsigned char*)(v.ptr()), v.size());
			}
			return 0;
		}
	};
}

template <typename Char = char>
using HxXStringView = ::HxLibs::XStringView<Char>;

using HxStringView = ::HxLibs::XStringView<char>;
using HxWStringView = ::HxLibs::XStringView<wchar_t>;

#else //__cplusplus

typedef HxCharConstSequence HxStringView;
typedef HxWCharConstSequence HxWStringView;

#endif //__cplusplus

#endif //!HX_STRING_VIEW

#endif //!HX_CORE_STRING_VIEW_H
