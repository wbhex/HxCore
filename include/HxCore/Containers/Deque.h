/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_DEQUE_H
#define HX_CORE_CONTAINERS_DEQUE_H

#ifdef __cplusplus
#ifndef HX_CONTAINERS_DEQUE
#define HX_CONTAINERS_DEQUE

#include <deque>
#include "DefaultAllocator.h"

namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Alloc = DefaultAllocator<T>>
		using Deque = ::std::deque<T, Alloc>;
	}
	template <typename T, typename Alloc = ContainerDefaultAllocator<T>>
	using Deque = ::std::deque<T, Alloc>;
}
template <typename T, typename Alloc = HxContainerDefaultAllocator<T>>
using HxDeque = ::std::deque<T, Alloc>;

#endif //!HX_CONTAINERS_DEQUE
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_DEQUE_H
