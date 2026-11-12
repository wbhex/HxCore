/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_FIXEDSIZE_POOL_BASE_H
#define HX_CORE_MEMORY_FIXEDSIZE_POOL_BASE_H

#ifdef __cplusplus
#ifndef HX_MEMORY_FIXEDSIZE_POOL_BASE
#define HX_MEMORY_FIXEDSIZE_POOL_BASE

#include "LockGuard.h"
#include "AtomicLocker.h"
#include "MemoryAlignment.h"
#include "FixedSizePoolFlags.h"
#include "FixedSizePoolState.h"
#include "MemoryPoolCacheList.h"
#include "MemoryGlobalDefault.h"

namespace HxLibs
{
	//表示固定大小缓存内存池对象。
	template <typename T = void, typename Global = MemoryGlobalDefault, size_t Alignment = MemoryAlignment<T>::value>
	class MemoryFixedSizePoolBase : Uncopyable
	{
		HX_STATIC_ASSERT((Alignment >= MemoryAlignment<T>::value), "::HxLibs::MemoryFixedSizePoolBase<> Alignment parameter must >= MemoryAlignment<T>::value.");
		HX_STATIC_ASSERT((checkPowerOf2(Alignment)), "::HxLibs::MemoryFixedSizePoolBase<> Alignment parameter must be a power of 2.");

	public:
		//表示缓存池状态。
		using State = HxFixedSizePoolState;
		//表示同步锁对象。
		using Locker = ::HxLibs::AtomicLocker;
		//表示同步锁定对象。
		using LockGuard = ::HxLibs::SpinLockGuard<Locker>;

	protected:
		//容量上限
		int m_limit;
		//缓存数量
		int m_cache;
		//活动数量
		int m_active{ 0 };
		//同步对象
		mutable Locker m_locker;
		//缓存大小
		size_t m_size;
		//字节大小
		size_t m_bufSize;
		//对齐大小
		size_t m_alignedSize;
		//释放计数
		long long m_free{ 0 };
		//分配计数
		long long m_alloc{ 0 };
		//复用计数
		long long m_reused{ 0 };
		//缓存集合
		MemoryPoolCacheList m_pool;

		//对齐大小
		HX_INLINE static size_t alignSize(size_t value) HX_NOEXCEPT
		{
			return (value + (Alignment - 1)) / Alignment * Alignment;
		}

		//从全局进行分配。。
		HX_INLINE T* allocGlobal(size_t bytes, LockGuard& locker) HX_NOEXCEPT
		{
			int limit = m_limit;
			if (limit <= 0 || m_active < limit)
			{
				++m_alloc;
				++m_active;
				locker.unlock();
				auto buf = (T*)Global::allocate(bytes);
				if (buf != nullptr)
				{
					return buf;
				}
				else
				{//分配失败，减去活动数
					locker.lock();
					--m_alloc;
					--m_active;
				}
			}
			return nullptr;
		}

		//分配缓存内容的具体实现。
		HX_INLINE T* allocCore(LockGuard& locker, bool* reused = nullptr) HX_NOEXCEPT
		{
			if (!m_pool.empty())
			{
				if (reused != nullptr)
				{
					*reused = true;
				}
				auto buf = m_pool.pop();
				++m_reused;
				++m_active;
				return (T*)buf;
			}
			else
			{
				if (reused != nullptr)
				{
					*reused = false;
				}
				return allocGlobal(m_bufSize, locker);
			}
		}

		//设置缓存大小的具体实现。
		HX_INLINE bool setSizeCore(size_t value, LockGuard& locker) HX_NOEXCEPT
		{
			if (m_size != value)
			{
				m_size = value;
				m_bufSize = value * Sizeof<T>::value;
				auto alignedSize = alignSize(value);
				if (m_alignedSize != alignedSize)
				{
					m_alignedSize = alignedSize;
					if (m_pool.count() > 0)
					{
						MemoryPoolCacheList list;
						m_pool.swap(list);
						//先解锁然后再释放内存
						locker.unlock();
						auto buf = list.pop();
						while (buf != nullptr)
						{
							Global::deallocate(buf);
							buf = list.pop();
						}
						return true;
					}
				}
			}
			return false;
		}
		//设置缓存数量的具体实现。
		HX_INLINE bool setCacheCore(int value, LockGuard& locker) HX_NOEXCEPT
		{
			if (m_cache != value)
			{
				m_cache = value;
				auto count = (size_t)value;
				if (m_pool.count() > count)
				{
					MemoryPoolCacheList list;
					if (count == 0)
					{
						m_pool.swap(list);
					}
					else
					{
						while (m_pool.count() > count)
						{
							list.push(m_pool.pop());
						}
					}
					//先解锁然后再释放内存
					locker.unlock();
					auto buf = list.pop();
					while (buf != nullptr)
					{
						Global::deallocate(buf);
						buf = list.pop();
					}
					return true;
				}
			}
			return false;
		}

		//初始化缓存池。
		HX_INLINE MemoryFixedSizePoolBase(int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_size(0), m_limit(limit), m_cache(cache < 0 ? 0 : cache), m_bufSize(0), m_alignedSize(0)
		{
			//NOTHING
		}
		//初始化缓存池。
		HX_INLINE MemoryFixedSizePoolBase(size_t size, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_size(size), m_limit(limit), m_cache(cache < 0 ? 0 : cache), m_bufSize(Sizeof<T>::value* m_size), m_alignedSize(alignSize(m_bufSize))
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		HX_INLINE ~MemoryFixedSizePoolBase() HX_NOEXCEPT
		{
			auto buf = m_pool.pop();
			while (buf != nullptr)
			{
				Global::deallocate(buf);
				buf = m_pool.pop();
			}
		}

	public:
		//获取缓存大小。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_size;
		}
		//获取缓存大小。
		HX_INLINE size_t bytes() const HX_NOEXCEPT
		{
			return m_bufSize;
		}

		//获取缓存上限。
		HX_INLINE int limit() const HX_NOEXCEPT
		{
			return m_limit;
		}
		//获取可缓存量。
		HX_INLINE int cache() const HX_NOEXCEPT
		{
			return m_cache;
		}
		//获取活动数量。
		HX_INLINE int active() const HX_NOEXCEPT
		{
			return m_active;
		}
		//获取缓存数量。
		HX_INLINE int cached() const HX_NOEXCEPT
		{
			return (int)m_pool.count();
		}

		//获取使用率（0.0 - 1.0）。
		HX_INLINE double usage() const HX_NOEXCEPT
		{
			int limit = m_limit;
			if (limit <= 0)
			{
				return 0.0;
			}
			double active = m_active;
			return active / limit;
		}

		//释放缓存内容。
		HX_INLINE void free(T* buf, size_t size) HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			--m_active;
			if (m_pool.count() < m_cache
				&& (size == m_size || alignSize(size * Sizeof<T>::value) == m_alignedSize))
			{
				m_pool.push(buf);
			}
			else
			{//提前解锁，然后再释放内存
				++m_free;
				lck.unlock();
				Global::deallocate(buf);
			}
		}

		//查询当前状态。
		HX_INLINE void query(State& state) const HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			state.size = m_size;
			state.limit = m_limit;
			state.cache = m_cache;
			state.active = m_active;
			state.cached = (int)m_pool.count();
			state.free = m_free;
			state.alloc = m_alloc;
			state.reused = m_reused;
		}
		//查询当前信息。
		HX_INLINE void query(int* limit, int* cache) const HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			if (limit != nullptr)
			{
				*limit = m_limit;
			}
			if (cache != nullptr)
			{
				*cache = m_cache;
			}
		}
		//查询当前信息。
		HX_INLINE void query(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			if (size != nullptr)
			{
				*size = m_size;
			}
			if (limit != nullptr)
			{
				*limit = m_limit;
			}
			if (cache != nullptr)
			{
				*cache = m_cache;
			}
		}

		//重置缓存上限和缓存数量。
		HX_INLINE void reset(int limit, int cache) HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			if (limit != HX_FIXEDSIZE_POOL_LIMIT_RESERVED)
			{
				m_limit = limit;
			}
			if (cache >= 0)
			{
				if (m_limit > 0 && cache > m_limit)
				{
					cache = m_limit;
				}
				setCacheCore(cache, lck);
			}
			else if (m_limit > 0 && m_cache > m_limit)
			{
				setCacheCore(m_limit, lck);
			}
		}
		//重置缓存池。
		HX_INLINE void reset(size_t size, int limit, int cache) HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			if (size != HX_FIXEDSIZE_POOL_SIZE_RESERVED)
			{
				if (setSizeCore(size, lck))
				{
					lck.lock();
				}
			}
			if (limit != HX_FIXEDSIZE_POOL_LIMIT_RESERVED)
			{
				m_limit = limit;
			}
			if (cache >= 0)
			{
				if (m_limit > 0 && cache > m_limit)
				{
					cache = m_limit;
				}
				setCacheCore(cache, lck);
			}
			else if (m_limit > 0 && m_cache > m_limit)
			{
				setCacheCore(m_limit, lck);
			}
		}

		//设置缓存大小。
		HX_INLINE void setSize(size_t value) HX_NOEXCEPT
		{
			if (value > 0)
			{
				LockGuard lck(m_locker);
				setSizeCore(value, lck);
			}
		}
		//设置缓存上限。
		HX_INLINE void setLimit(int value) HX_NOEXCEPT
		{
			LockGuard lck(m_locker);
			m_limit = value;
			if (m_limit > 0 && m_cache > m_limit)
			{
				setCacheCore(m_limit, lck);
			}
		}
		//设置可缓存数。
		HX_INLINE void setCache(int value) HX_NOEXCEPT
		{
			if (value >= 0)
			{
				LockGuard lck(m_locker);
				if (m_limit > 0 && value > m_limit)
				{
					value = m_limit;
				}
				setCacheCore(value, lck);
			}
		}
	};
}

#endif //!HX_MEMORY_FIXEDSIZE_POOL_BASE
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_FIXEDSIZE_POOL_BASE_H
