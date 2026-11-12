/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_LINKED_LIST_H
#define HX_CORE_CONTAINERS_LINKED_LIST_H

#ifdef __cplusplus
#include <list>
#include "DefaultAllocator.h"
#ifndef HX_CONTAINERS_LINKED_LIST
#define HX_CONTAINERS_LINKED_LIST
namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Alloc = DefaultAllocator<T>>
		using LinkedList = ::std::list<T, Alloc>;
	}
	template <typename T, typename Alloc = ContainerDefaultAllocator<T>>
	using LinkedList = ::std::list<T, Alloc>;
}
template <typename T, typename Alloc = HxContainerDefaultAllocator<T>>
using HxLinkedList = ::std::list<T, Alloc>;
#endif //!HX_CONTAINERS_LINKED_LIST

#ifndef HX_CONTAINERS_LIST
#define HX_CONTAINERS_LIST
namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Alloc = DefaultAllocator<T>>
		using List = ::std::list<T, Alloc>;
	}
}
#endif //!HX_CONTAINERS_LIST
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_LINKED_LIST_H
