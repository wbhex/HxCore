/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_FIXEDSIZE_POOL_RESOURCE_H
#define HX_CORE_MEMORY_FIXEDSIZE_POOL_RESOURCE_H

#ifdef __cplusplus
#ifndef HX_MEMORY_FIXEDSIZE_POOL_RESOURCE
#define HX_MEMORY_FIXEDSIZE_POOL_RESOURCE

#include "MemoryResource.h"
#include "MemoryFixedSizePoolBase.h"
#if _HAS_CXX17
#include <memory_resource>
#endif //_HAS_CXX17

namespace HxLibs
{
	//表示可自动调整大小的固定大小内存池资源对象，如果分配的大小不满足固定大小要求则会调整固定内存大小。
	template <typename Global = MemoryGlobalDefault, size_t Alignment = MemoryAlignment<void>::value>
	class MemoryFixedSizePoolAutoSizeResource : public MemoryFixedSizePoolBase<unsigned char, Global, Alignment>
		, public MemoryResource
#if _HAS_CXX17
		, public ::std::pmr::memory_resource
#endif //_HAS_CXX17
	{
		using _MyBase = MemoryFixedSizePoolBase<unsigned char, Global, Alignment>;
		using LockGuard = typename _MyBase::LockGuard;

	public:
		//表示字节类型。
		using Byte = unsigned char;
		//表示缓存池状态。
		using State = FixedSizePoolState;

	public:
		//初始化缓存池。
		HX_INLINE MemoryFixedSizePoolAutoSizeResource(int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(limit, cache)
		{
			//NOTHING
		}

		//分配缓存内容。
		HX_INLINE Byte* alloc(size_t size, bool* reused = nullptr) HX_NOEXCEPT
		{
			do
			{
				LockGuard lck(_MyBase::m_locker);
				if (_MyBase::setSizeCore(size, lck))
				{
					continue;
				}
				return _MyBase::allocCore(lck, reused);
			} while (true);
		}

		//重置缓存上限和缓存数量。
		HX_INLINE void reset(int limit, int cache) HX_NOEXCEPT
		{
			LockGuard lck(_MyBase::m_locker);
			if (limit != HX_FIXEDSIZE_POOL_LIMIT_RESERVED)
			{
				_MyBase::m_limit = limit;
			}
			if (cache >= 0)
			{
				if (_MyBase::m_limit > 0 && cache > _MyBase::m_limit)
				{
					cache = _MyBase::m_limit;
				}
				_MyBase::setCacheCore(cache, lck);
			}
			else if (_MyBase::m_limit > 0 && _MyBase::m_cache > _MyBase::m_limit)
			{
				_MyBase::setCacheCore(_MyBase::m_limit, lck);
			}
		}

		//分配指定大小的内存
		virtual void* HX_STDCALL allocate(size_t bytes, size_t align) HX_NOEXCEPT override
		{
			return alloc(bytes);
		}
		//释放之前分配的内存
		virtual void HX_STDCALL deallocate(void* ptr, size_t bytes, size_t align) HX_NOEXCEPT override
		{
			_MyBase::free((Byte*)ptr, bytes);
		}

#if _HAS_CXX17
	private:
		virtual void* do_allocate(size_t bytes, size_t align) override
		{
			return alloc(bytes);
		}
		virtual void do_deallocate(void* ptr, size_t bytes, size_t align) override
		{
			_MyBase::free((Byte*)ptr, bytes);
		}
		virtual bool do_is_equal(const ::std::pmr::memory_resource& other) const noexcept override
		{
			return &other == (::std::pmr::memory_resource*)this;
		}
#endif //_HAS_CXX17
	};

	//表示手动设置大小的固定大小内存池资源对象，如果分配的大小不满足固定大小要求则会从全局内存中分配。
	template <typename Global = MemoryGlobalDefault, size_t Alignment = MemoryAlignment<void>::value>
	class MemoryFixedSizePoolManualSizeResource : public MemoryFixedSizePoolBase<unsigned char, Global, Alignment>
		, public MemoryResource
#if _HAS_CXX17
		, public ::std::pmr::memory_resource
#endif //_HAS_CXX17
	{
		using _MyBase = MemoryFixedSizePoolBase<unsigned char, Global, Alignment>;
		using LockGuard = typename _MyBase::LockGuard;

	public:
		//表示字节类型。
		using Byte = unsigned char;
		//表示缓存池状态。
		using State = FixedSizePoolState;

	public:
		//初始化缓存池。
		HX_INLINE MemoryFixedSizePoolManualSizeResource(size_t size, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(size, limit, cache)
		{
			//NOTHING
		}

		//分配缓存内容。
		HX_INLINE Byte* alloc(size_t size, bool* reused = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(_MyBase::m_locker);
			if (_MyBase::m_size == size || _MyBase::alignSize(size) == _MyBase::m_alignedSize)
			{//从固定大小缓存区进行分配
				return _MyBase::allocCore(lck, reused);
			}
			else
			{//不从固定大小缓存区进行分配
				if (reused != nullptr)
				{
					*reused = false;
				}
				return _MyBase::allocGlobal(_MyBase::alignSize(size), lck);
			}
		}

		//分配指定大小的内存
		virtual void* HX_STDCALL allocate(size_t bytes, size_t align) HX_NOEXCEPT override
		{
			return alloc(bytes);
		}
		//释放之前分配的内存
		virtual void HX_STDCALL deallocate(void* ptr, size_t bytes, size_t align) HX_NOEXCEPT override
		{
			_MyBase::free((Byte*)ptr, bytes);
		}

#if _HAS_CXX17
	private:
		virtual void* do_allocate(size_t bytes, size_t align) override
		{
			return alloc(bytes);
		}
		virtual void do_deallocate(void* ptr, size_t bytes, size_t align) override
		{
			_MyBase::free((Byte*)ptr, bytes);
		}
		virtual bool do_is_equal(const ::std::pmr::memory_resource& other) const noexcept override
		{
			return &other == (::std::pmr::memory_resource*)this;
		}
#endif //_HAS_CXX17
	};

	template <typename Global = MemoryGlobalDefault, size_t Alignment = MemoryAlignment<void>::value>
	using MemoryFixedSizePoolResource = MemoryFixedSizePoolAutoSizeResource<Global, Alignment>;
}

template <typename Global = HxMemoryGlobalDefault, size_t Alignment = HxMemoryAlignment<void>::value>
using HxMemoryFixedSizePoolResource = ::HxLibs::MemoryFixedSizePoolAutoSizeResource<Global, Alignment>;

template <typename Global = HxMemoryGlobalDefault, size_t Alignment = HxMemoryAlignment<void>::value>
using HxMemoryFixedSizePoolAutoSizeResource = ::HxLibs::MemoryFixedSizePoolAutoSizeResource<Global, Alignment>;

template <typename Global = HxMemoryGlobalDefault, size_t Alignment = HxMemoryAlignment<void>::value>
using HxMemoryFixedSizePoolManualSizeResource = ::HxLibs::MemoryFixedSizePoolManualSizeResource<Global, Alignment>;

#endif //!HX_MEMORY_FIXEDSIZE_POOL_RESOURCE
#endif //__cplusplus

#endif // !HX_CORE_MEMORY_FIXEDSIZE_POOL_RESOURCE_H
