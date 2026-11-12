/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_VECTOR_H
#define HX_CORE_CONTAINERS_VECTOR_H

#ifdef __cplusplus
#ifndef HX_CONTAINERS_VECTOR
#define HX_CONTAINERS_VECTOR

#include <vector>
#include "DefaultAllocator.h"

namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Alloc = DefaultAllocator<T>>
		using Vector = ::std::vector<T, Alloc>;
	}
	template <typename T, typename Alloc = ContainerDefaultAllocator<T>>
	using Vector = ::std::vector<T, Alloc>;
}
template <typename T, typename Alloc = HxContainerDefaultAllocator<T>>
using HxVector = ::std::vector<T, Alloc>;

#endif //!HX_CONTAINERS_VECTOR
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_VECTOR_H
