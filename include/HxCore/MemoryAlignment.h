/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_ALIGNMENT_H
#define HX_CORE_MEMORY_ALIGNMENT_H

#ifdef __cplusplus
#ifndef HX_MEMORY_ALIGNMENT
#define HX_MEMORY_ALIGNMENT

#include "Defines.h"

namespace HxLibs
{
	template <typename T>
	struct MemoryAlignment
	{
		static HX_CONSTEXPR size_t value = Alignof<T>::value > Alignof<void*>::value ? Alignof<T>::value : Alignof<void*>::value;
	};
	template <>
	struct MemoryAlignment<void>
	{
		static HX_CONSTEXPR size_t value = Alignof<void*>::value;
	};
}
template <typename T>
using HxMemoryAlignment = ::HxLibs::MemoryAlignment<T>;

#endif //!HX_MEMORY_ALIGNMENT
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_ALIGNMENT_H
