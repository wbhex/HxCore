/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_FIXEDSIZE_POOL_H
#define HX_CORE_MEMORY_FIXEDSIZE_POOL_H

#ifdef __cplusplus
#ifndef HX_MEMORY_FIXEDSIZE_POOL
#define HX_MEMORY_FIXEDSIZE_POOL

#include "MemoryFixedSizePoolBase.h"

namespace HxLibs
{
	//表示固定大小缓存内存池对象。
	template <typename T = unsigned char, typename Global = MemoryGlobalDefault, size_t Alignment = MemoryAlignment<T>::value>
	class MemoryFixedSizePool : public MemoryFixedSizePoolBase<T, Global, Alignment>
	{
		using _MyBase = MemoryFixedSizePoolBase<T, Global, Alignment>;
		using LockGuard = typename _MyBase::LockGuard;

	public:
		//表示缓存池状态。
		using State = FixedSizePoolState;

	public:
		//初始化缓存池。
		HX_INLINE MemoryFixedSizePool(size_t size = 1, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(size == 0 ? 1 : size, limit, cache)
		{
			//NOTHING
		}

		//分配缓存内容。
		HX_INLINE T* alloc(bool* reused = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(_MyBase::m_locker);
			return _MyBase::allocCore(lck, reused);
		}
		//分配缓存内容。
		HX_INLINE T* alloc(size_t& size, bool* reused = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(_MyBase::m_locker);
			size = _MyBase::m_size;
			return _MyBase::allocCore(lck, reused);
		}
		//分配缓存内容。
		HX_INLINE T* alloc(size_t& size, size_t& bytes, bool* reused = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(_MyBase::m_locker);
			size = _MyBase::m_size;
			bytes = _MyBase::m_bufSize;
			return _MyBase::allocCore(lck, reused);
		}
	};
}
template <typename T = unsigned char, typename Global = HxMemoryGlobalDefault, size_t Alignment = HxMemoryAlignment<T>::value>
using HxMemoryFixedSizePool = ::HxLibs::MemoryFixedSizePool<T, Global, Alignment>;

#endif //!HX_MEMORY_FIXEDSIZE_POOL
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_FIXEDSIZE_POOL_H
