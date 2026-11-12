/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_HASH_MAP_H
#define HX_CORE_CONTAINERS_HASH_MAP_H

#ifdef __cplusplus
#include <unordered_map>
#include "DefaultAllocator.h"
#ifndef HX_CONTAINERS_HASH_MAP
#define HX_CONTAINERS_HASH_MAP
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = DefaultAllocator<::std::pair<const Key, Val>>>
		using HashMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
	}
	template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = ContainerDefaultAllocator<::std::pair<const Key, Val>>>
	using HashMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
}
template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = HxContainerDefaultAllocator<::std::pair<const Key, Val>>>
using HxHashMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
#endif //!HX_CONTAINERS_HASH_MAP

#ifndef HX_CONTAINERS_UNORDERED_MAP
#define HX_CONTAINERS_UNORDERED_MAP
namespace HxLibs
{
	namespace Containers
	{
		template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = DefaultAllocator<::std::pair<const Key, Val>>>
		using UnorderedMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
	}
	template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = ContainerDefaultAllocator<::std::pair<const Key, Val>>>
	using UnorderedMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
}
template <typename Key, typename Val, typename Hasher = ::std::hash<Key>, typename Keyseq = ::std::equal_to<Key>, typename Alloc = HxContainerDefaultAllocator<::std::pair<const Key, Val>>>
using HxUnorderedMap = ::std::unordered_map<Key, Val, Hasher, Keyseq, Alloc>;
#endif //!HX_CONTAINERS_UNORDERED_MAP
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_HASH_MAP_H
