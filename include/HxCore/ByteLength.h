/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_BYTE_LENGTH_H
#define HX_CORE_BYTE_LENGTH_H

#include "Defines.h"

#ifdef __cplusplus
#ifndef HX_BYTE_LENGTH
#define HX_BYTE_LENGTH

namespace HxLibs
{
	template <typename T>
	struct ByteLength
	{
		static HX_CONSTEXPR size_t value = sizeof(T);

		HX_CONSTEXPR11_FUNC static size_t count(size_t size) HX_NOEXCEPT
		{
			return size * sizeof(T);
		}
	};
	template <>
	struct ByteLength<void>
	{
		static HX_CONSTEXPR size_t value = 1;

		HX_CONSTEXPR11_FUNC static size_t count(size_t size) HX_NOEXCEPT
		{
			return size;
		}
	};
}
template <typename T>
using HxByteLength = ::HxLibs::ByteLength<T>;

#endif //!HX_BYTE_LENGTH
#endif //__cplusplus

#endif //!HX_CORE_BYTE_LENGTH_H
