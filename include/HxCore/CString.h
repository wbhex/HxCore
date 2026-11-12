/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CSTRING_H
#define HX_CORE_CSTRING_H

#ifndef HX_CSTRING
#define HX_CSTRING

#ifdef __cplusplus
#include <assert.h>
#include "StringView.h"
#include "CStringLength.h"
namespace HxLibs
{
	//表示C样式字符串，为一种特殊的字符串视图，要求字符串必须以0结尾，长度不包含0。
	template <typename Char = char>
	struct XCString : public XStringView<Char>
	{
		using _MyBase = XStringView<Char>;

	public:
		//构造函数。
		HX_CONSTEXPR11_FUNC XCString() HX_NOEXCEPT
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC XCString(::std::nullptr_t) HX_NOEXCEPT
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC XCString(const Char* v) HX_NOEXCEPT
			: _MyBase(v)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC XCString(const Char* v, size_t l) HX_NOEXCEPT
			: _MyBase(v, l)
		{
			assert(l == CStringLength::count(v));
		}
		//构造函数。
		template <typename Traits, typename Alloc>
		HX_INLINE XCString(const XString<Char, Traits, Alloc>& v) HX_NOEXCEPT
			: _MyBase(v.c_str(), v.size())
		{
			//NOTHONG
		}

		//获取C字符串。
		HX_CONSTEXPR11_FUNC const Char* c_str() const HX_NOEXCEPT
		{
			return _MyBase::m_ptr;
		}
	};

	using CString = XCString<char>;
	using CWString = XCString<wchar_t>;
}

template <typename T>
using HxXCString = ::HxLibs::XCString<T>;

using HxCString = ::HxLibs::XCString<char>;
using HxCWString = ::HxLibs::XCString<wchar_t>;

namespace std
{ 
	//定义标准库哈希
	template <typename Char>
	struct hash<::HxLibs::XCString<Char>>
	{
		using result_type = size_t;
		using argument_type = ::HxLibs::XCString<Char>;

		HX_CONSTEXPR17_FUNC size_t operator()(const ::HxLibs::XCString<Char>& v) const HX_NOEXCEPT
		{
			if (v.size() > 0)
			{
				return ::HxHash_fnv1a((const unsigned char*)(v.ptr()), v.size());
			}
			return 0;
		}
	};
}
#else //__cplusplus
typedef HxCharConstSequence HxCString;
typedef HxWCharConstSequence HxCWString;
#endif //__cplusplus
#endif //!HX_CSTRING

#endif //!HX_CORE_CSTRING_H
