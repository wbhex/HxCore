/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRING_POOL_DEFALUT_H
#define HX_CORE_STRING_POOL_DEFALUT_H

#ifndef HX_STRING_POOL_DEFALUT
#define HX_STRING_POOL_DEFALUT

#include "API.h"
#include "StringPool.h"

 //{8CD434A3-F54D-47B4-9323-A51A6F87C9DE}
#define HX_STRING_POOL_DEFAULT_FUNC 0x8cd434a3, 0xf54d, 0x47b4, 0x93, 0x23, 0xa5, 0x1a, 0x6f, 0x87, 0xc9, 0xde
typedef HxStringPool* (HX_STDCALL* HxStringPoolDefaultFunc)();

//{993C2AE4-EA44-4FDA-B4AF-671B3C559951}
#define HX_WSTRING_POOL_DEFAULT_FUNC 0x993c2ae4, 0xea44, 0x4fda, 0xb4, 0xaf, 0x67, 0x1b, 0x3c, 0x55, 0x99, 0x51
typedef HxWStringPool* (HX_STDCALL* HxWStringPoolDefaultFunc)();

#ifdef __cplusplus

namespace HxLibs
{
	//表示全局默认字符串池。
	template <typename Char = char>
	struct XStringPoolDefault
	{
	private:
		static XStringPool<Char>* get() HX_NOEXCEPT;

	public:
		static XStringPool<Char>* current() HX_NOEXCEPT
		{
			static XStringPool<Char>* _val = get();
			return _val;
		}

		HX_INLINE static XCString<Char> retrieve(const XCString<Char>& value) HX_NOEXCEPT
		{
			return current()->retrieve(value);
		}
	};

	template <>
	HX_INLINE XStringPool<char>* XStringPoolDefault<char>::get() HX_NOEXCEPT
	{
		auto func = queryFunction<HxStringPoolDefaultFunc>(Uuid(HX_STRING_POOL_DEFAULT_FUNC));
		return func();
	}
	template <>
	HX_INLINE XStringPool<wchar_t>* XStringPoolDefault<wchar_t>::get() HX_NOEXCEPT
	{
		auto func = queryFunction<HxWStringPoolDefaultFunc>(Uuid(HX_WSTRING_POOL_DEFAULT_FUNC));
		return func();
	}

	using StringPoolDefault = XStringPoolDefault<char>;
	using WStringPoolDefault = XStringPoolDefault<wchar_t>;
}
template <typename Char = char>
using HxXStringPoolDefault = ::HxLibs::XStringPoolDefault<Char>;

using HxStringPoolDefault = ::HxLibs::XStringPoolDefault<char>;
using HxWStringPoolDefault = ::HxLibs::XStringPoolDefault<wchar_t>;

#endif //__cplusplus

#endif //HX_STRING_POOL_DEFALUT

#endif //!HX_CORE_STRING_POOL_DEFALUT_H
