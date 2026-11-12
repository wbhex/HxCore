/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRING_POOL_H
#define HX_CORE_STRING_POOL_H

#ifndef HX_STRING_POOL
#define HX_STRING_POOL

#include "CString.h"
#include "Interface.h"

#ifdef __cplusplus
namespace HxLibs
{ 
	//表示字符串池对象。
	template <typename Char = char>
	HX_INTERFACE_DECLARE(XStringPool)
	{
		//取得缓存的字符串。
		HX_METHOD(XCString<Char>, retrieve)(const XCString<Char>*) HX_NOEXCEPT = 0;

		//取得缓存的字符串。
		HX_INLINE XCString<Char> retrieve(const XCString<Char>&value) HX_NOEXCEPT
		{
			return retrieve(&value);
		}
	};
	using StringPool = XStringPool<char>;
	using WStringPool = XStringPool<wchar_t>;
}
template <typename Char = char>
using HxXStringPool = ::HxLibs::XStringPool<Char>;
using HxStringPool = ::HxLibs::XStringPool<char>;
using HxWStringPool = ::HxLibs::XStringPool<wchar_t>;
#else //!__cplusplus
#define HX_STRING_POOL_DECLARE(NAME, TYPE)\
HX_INTERFACE_DECLARE(NAME)\
{\
	HX_METHOD(TYPE, retrieve)(HX_THIS_(NAME) const TYPE*) HX_NOEXCEPT HX_PURE;\
};
HX_STRING_POOL_DECLARE(HxStringPool, HxCString);
HX_STRING_POOL_DECLARE(HxWStringPool, HxCWString);
#endif //!__cplusplus

#endif //!HX_STRING_POOL

#endif //!HX_CORE_STRING_POOL_H
