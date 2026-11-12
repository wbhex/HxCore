/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_HASH_SET_H
#define HX_CORE_CONTAINERS_HASH_SET_H

#ifdef __cplusplus
#include <unordered_set>
#include "DefaultAllocator.h"
#ifndef HX_CONTAINERS_HASH_SET
#define HX_CONTAINERS_HASH_SET
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = DefaultAllocator<Key>>
		using HashSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
	}
	template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = ContainerDefaultAllocator<Key>>
	using HashSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
}
template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = HxContainerDefaultAllocator<Key>>
using HxHashSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
#endif //!HX_CONTAINERS_HASH_SET

#ifndef HX_CONTAINERS_UNORDERED_SET
#define HX_CONTAINERS_UNORDERED_SET
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = DefaultAllocator<Key>>
		using UnorderedSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
	}
	template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = ContainerDefaultAllocator<Key>>
	using UnorderedSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
}
template <typename Key, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = HxContainerDefaultAllocator<Key>>
using HxUnorderedSet = ::std::unordered_set<Key, Hasher, Keyseq, Alloc>;
#endif //!HX_CONTAINERS_UNORDERED_SET
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_HASH_SET_H
