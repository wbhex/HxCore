/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_RBTREE_MAP_H
#define HX_CORE_CONTAINERS_RBTREE_MAP_H

#ifdef __cplusplus
#include <map>
#include "DefaultAllocator.h"
#ifndef HX_CONTAINERS_RBTREE_MAP
#define HX_CONTAINERS_RBTREE_MAP
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Val, typename Less = ::std::less<Key>, typename Alloc = DefaultAllocator<::std::pair<const Key, Val>>>
		using RBTreeMap = ::std::map<Key, Val, Less, Alloc>;
	}
	template <typename Key, typename Val, typename Less = ::std::less<Key>, typename Alloc = ContainerDefaultAllocator<::std::pair<const Key, Val>>>
	using RBTreeMap = ::std::map<Key, Val, Less, Alloc>;
}
template <typename Key, typename Val, typename Less = ::std::less<Key>, typename Alloc = HxContainerDefaultAllocator<::std::pair<const Key, Val>>>
using HxRBTreeMap = ::std::map<Key, Val, Less, Alloc>;
#endif //!HX_CONTAINERS_RBTREE_MAP

#ifndef HX_CONTAINERS_MAP
#define HX_CONTAINERS_MAP
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Val, typename Less = ::std::less<Key>, typename Alloc = DefaultAllocator<::std::pair<const Key, Val>>>
		using Map = ::std::map<Key, Val, Less, Alloc>;
	}
}
#endif //!HX_CONTAINERS_MAP
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_RBTREE_MAP_H
