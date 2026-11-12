/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_RBTREE_SET_H
#define HX_CORE_CONTAINERS_RBTREE_SET_H

#ifdef __cplusplus
#include <set>
#include "DefaultAllocator.h"
#ifndef HX_CONTAINERS_RBTREE_SET
#define HX_CONTAINERS_RBTREE_SET
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Less = ::std::less<Key>, typename Alloc = DefaultAllocator<Key>>
		using RBTreeSet = ::std::set<Key, Less, Alloc>;
	}
	template <typename Key, typename Less = ::std::less<Key>, typename Alloc = ContainerDefaultAllocator<Key>>
	using RBTreeSet = ::std::set<Key, Less, Alloc>;
}
template <typename Key, typename Less = ::std::less<Key>, typename Alloc = HxContainerDefaultAllocator<Key>>
using HxRBTreeSet = ::std::set<Key, Less, Alloc>;
#endif //!HX_CONTAINERS_RBTREE_SET

#ifndef HX_CONTAINERS_SET
#define HX_CONTAINERS_SET
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Less = ::std::less<Key>, typename Alloc = DefaultAllocator<Key>>
		using Set = ::std::set<Key, Less, Alloc>;
	}
}
#endif //!HX_CONTAINERS_SET
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_RBTREE_SET_H
