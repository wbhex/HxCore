/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CSTRING_LENGTH_H
#define HX_CORE_CSTRING_LENGTH_H

#ifdef __cplusplus
#ifndef HX_CSTRING_LENGTH
#define HX_CSTRING_LENGTH

#include <string>
#include "Defines.h"

namespace HxLibs
{
	struct CStringLength
	{
		template <typename Char = char>
		HX_CONSTEXPR17_FUNC static size_t count(const Char* v) HX_NOEXCEPT
		{
			return v == nullptr ? (size_t)0 : ::std::char_traits<Char>::length(v);
		}
	};
}
using HxCStringLength = ::HxLibs::CStringLength;

#endif //!HX_CSTRING_LENGTH
#endif //__cplusplus

#endif //!HX_CORE_CSTRING_LENGTH_H
