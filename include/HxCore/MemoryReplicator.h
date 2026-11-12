/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_REPLICATOR_H
#define HX_CORE_MEMORY_REPLICATOR_H

#ifdef __cplusplus
#ifndef HX_MEMORY_REPLICATOR
#define HX_MEMORY_REPLICATOR

#include "Defines.h"

#ifndef HX_MEMORY_REPLICATOR_FUNC
#include <cstring>
#define HX_MEMORY_REPLICATOR_FUNC(src, dest, size) ::memcpy(dest, src, size)
#endif //!HX_MEMORY_REPLICATOR_FUNC

namespace HxLibs
{
	//表示内存复制对象。
	class MemoryReplicator
	{
	public:
		HX_INLINE static void replicate(const void* src, void* dest, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(src, dest, size);
		}
		HX_INLINE static void replicate(const void* src, size_t srcOffset, void* dest, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(((unsigned char*)src + srcOffset), dest, size);
		}
		HX_INLINE static void replicate(const void* src, void* dest, size_t destOffset, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(src, ((unsigned char*)dest + destOffset), size);
		}
		HX_INLINE static void replicate(const void* src, size_t srcOffset, void* dest, size_t destOffset, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(((unsigned char*)src + srcOffset), ((unsigned char*)dest + destOffset), size);
		}

		template <typename T>
		HX_INLINE static void replicate(const T* src, void* dest, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(src, dest, (sizeof(T) * size));
		}
		template <typename T>
		HX_INLINE static void replicate(const T* src, size_t srcOffset, T* dest, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC((src + srcOffset), dest, (sizeof(T) * size));
		}
		template <typename T>
		HX_INLINE static void replicate(const T* src, T* dest, size_t destOffset, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC(src, (dest + destOffset), (sizeof(T) * size));
		}
		template <typename T>
		HX_INLINE static void replicate(const T* src, size_t srcOffset, T* dest, size_t destOffset, size_t size) HX_NOEXCEPT
		{
			HX_MEMORY_REPLICATOR_FUNC((src + srcOffset), (dest + destOffset), (sizeof(T) * size));
		}
	};
}
using HxMemoryReplicator = ::HxLibs::MemoryReplicator;

#endif //!HX_MEMORY_REPLICATOR
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_REPLICATOR_H
