/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SHARED_FIXEDSIZE_BUFFER_POOL_H
#define HX_CORE_SHARED_FIXEDSIZE_BUFFER_POOL_H

#ifndef HX_SHARED_FIXEDSIZE_BUFFER_POOL
#define HX_SHARED_FIXEDSIZE_BUFFER_POOL

#ifdef __cplusplus
#include <functional>
#include "Memory.h"
#include "MemoryFixedSizePool.h"
#include "MemoryResourceAllocator.h"
#include "MemoryFixedSizePoolResource.h"
#include "SharedBufferPoolAllocator.h"
#include "FixedSizePoolFlags.h"
#include "FixedSizePoolState.h"

namespace HxLibs
{
	template <typename T, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault, size_t BufMemAlignment = MemoryAlignment<T>::value>
	class SharedFixedSizeBufferPoolX
	{
	public:
		//表示状态。
		using State = FixedSizePoolState;
		//对象内存池。
		using ObjMemPool = MemoryFixedSizePoolResource<ObjMemGlobal>;
		//缓存内存池。
		using BufMemPool = MemoryFixedSizePool<T, BufMemGlobal, BufMemAlignment>;

	protected:
		//对象内存池
		::std::shared_ptr<ObjMemPool> m_objMemPool;
		//缓存内存池
		::std::shared_ptr<BufMemPool> m_bufMemPool;

		//初始化缓存池对象。
		HX_INLINE SharedFixedSizeBufferPoolX(size_t size = 1, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_bufMemPool(Memory::makeSharedObject<BufMemPool>(size, limit, cache))
			, m_objMemPool(Memory::makeSharedObject<ObjMemPool>(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache))
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		HX_INLINE ~SharedFixedSizeBufferPoolX() HX_NOEXCEPT
		{
			if (m_bufMemPool)
			{
				m_bufMemPool->setCache(0);
			}
			if (m_objMemPool)
			{
				m_objMemPool->setCache(0);
			}
		}

		//移动构造函数
		HX_INLINE SharedFixedSizeBufferPoolX(SharedFixedSizeBufferPoolX&& other) HX_NOEXCEPT
			: m_objMemPool(::std::move(other.m_objMemPool))
			, m_bufMemPool(::std::move(other.m_bufMemPool))
		{
			//NOTHING
		}
		//复制构造函数
		HX_INLINE SharedFixedSizeBufferPoolX(const SharedFixedSizeBufferPoolX& other) HX_NOEXCEPT
			: m_objMemPool(other.m_objMemPool)
			, m_bufMemPool(other.m_bufMemPool)
		{
			//NOTHING
		}

		//执行交换处理。
		HX_INLINE void swap(SharedFixedSizeBufferPoolX& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_objMemPool.swap(other.m_objMemPool);
				m_bufMemPool.swap(other.m_bufMemPool);
			}
		}

	public:
		//获取对象内存池对象
		HX_INLINE explicit operator ::std::shared_ptr<ObjMemPool>& () HX_NOEXCEPT
		{
			return m_objMemPool;
		}
		HX_INLINE explicit operator const ::std::shared_ptr<ObjMemPool>& () const HX_NOEXCEPT
		{
			return m_objMemPool;
		}
		//获取缓存内存池对象
		HX_INLINE explicit operator ::std::shared_ptr<BufMemPool>& () HX_NOEXCEPT
		{
			return m_bufMemPool;
		}
		HX_INLINE explicit operator const ::std::shared_ptr<BufMemPool>& () const HX_NOEXCEPT
		{
			return m_bufMemPool;
		}

		//获取缓存大小。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_bufMemPool->size();
		}
		//获取缓存字节大小。
		HX_INLINE size_t bytes() const HX_NOEXCEPT
		{
			return m_bufMemPool->bytes();
		}

		//获取缓存上限。
		HX_INLINE int limit() const HX_NOEXCEPT
		{
			return m_bufMemPool->limit();
		}
		//获取可缓存量。
		HX_INLINE int cache() const HX_NOEXCEPT
		{
			return m_bufMemPool->cache();
		}
		//获取活动数量。
		HX_INLINE int active() const HX_NOEXCEPT
		{
			return m_bufMemPool->active();
		}
		//获取缓存数量。
		HX_INLINE int cached() const HX_NOEXCEPT
		{
			return m_bufMemPool->cached();
		}

		//获取使用率（0.0 - 1.0）。
		HX_INLINE double usage() const HX_NOEXCEPT
		{
			return m_bufMemPool->usage();
		}

		//重置缓存。
		HX_INLINE void reset(size_t size, int limit = HX_FIXEDSIZE_POOL_LIMIT_RESERVED, int cache = HX_FIXEDSIZE_POOL_CACHE_RESERVED) HX_NOEXCEPT
		{
			m_bufMemPool->reset(size, limit, cache);
			m_objMemPool->setCache(cache);
		}

		//查询当前信息。
		HX_INLINE void queryInfo(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
		{
			m_bufMemPool->query(size, limit, cache);
		}
		//查询当前信息。
		HX_INLINE void query(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
		{
			queryInfo(size, limit, cache);
		}

		//查询当前状态。
		HX_INLINE void queryState(State* state) const HX_NOEXCEPT
		{
			if (state != nullptr)
			{
				m_bufMemPool->query(*state);
			}
		}
		//查询当前状态。
		HX_INLINE void queryState(State& state) const HX_NOEXCEPT
		{
			m_bufMemPool->query(state);
		}
		//查询当前状态。
		HX_INLINE void query(State* state) const HX_NOEXCEPT
		{
			queryState(state);
		}
		//查询当前状态。
		HX_INLINE void query(State& state) const HX_NOEXCEPT
		{
			queryState(state);
		}

		//设置缓存大小。
		HX_INLINE void setSize(size_t value) HX_NOEXCEPT
		{
			if (value > 0)
			{
				m_bufMemPool->setSize(value);
			}
		}
		//设置缓存上限。
		HX_INLINE void setLimit(int value) HX_NOEXCEPT
		{
			m_bufMemPool->setLimit(value);
		}
		//设置可缓存数。
		HX_INLINE void setCache(int value) HX_NOEXCEPT
		{
			m_bufMemPool->setCache(value);
			m_objMemPool->setCache(value);
		}
	};

	template <typename T, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault, size_t BufMemAlignment = MemoryAlignment<T>::value>
	class SharedFixedSizeBufferPool0 : public SharedFixedSizeBufferPoolX<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>
	{
		using _MyBase = SharedFixedSizeBufferPoolX<T, BufMemGlobal, ObjMemGlobal>;

	public:
		//表示状态。
		using State = FixedSizePoolState;
		//对象内存池。
		using ObjMemPool = MemoryFixedSizePoolResource<ObjMemGlobal>;
		//缓存内存池。
		using BufMemPool = MemoryFixedSizePool<T, BufMemGlobal, BufMemAlignment>;

	private:
		//表示删除器
		struct Deleter
		{
			size_t size;
			::std::shared_ptr<BufMemPool> pool;

			//构造函数
			HX_INLINE Deleter(size_t size, const ::std::shared_ptr<BufMemPool>& pool) HX_NOEXCEPT
				: size(size), pool(pool)
			{
				//NOTHING
			}

			//执行函数
			HX_INLINE void operator()(T* ptr) HX_NOEXCEPT
			{
				pool->free(ptr, size);
			}
		};
		//表示内存分配器
		using MemAlloc = SharedBufferPoolAllocator<char, ObjMemPool>;

	public:
		//初始化缓存池对象。
		HX_INLINE SharedFixedSizeBufferPool0(size_t size = 0, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(size, limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedFixedSizeBufferPool0(SharedFixedSizeBufferPool0&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动操作符
		HX_INLINE SharedFixedSizeBufferPool0& operator=(SharedFixedSizeBufferPool0&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_objMemPool = ::std::move(other.m_objMemPool);
				_MyBase::m_bufMemPool = ::std::move(other.m_bufMemPool);
			}
			return *this;
		}

		//复制构造函数
		HX_INLINE SharedFixedSizeBufferPool0(const SharedFixedSizeBufferPool0& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//复制操作符
		HX_INLINE SharedFixedSizeBufferPool0& operator=(const SharedFixedSizeBufferPool0& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_objMemPool = other.m_objMemPool;
				_MyBase::m_bufMemPool = other.m_bufMemPool;
			}
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(SharedFixedSizeBufferPool0& other) HX_NOEXCEPT
		{
			_MyBase::swap(other);
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool& reused) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, &reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool* reused = nullptr) HX_NOEXCEPT
		{
			return alloc(reused);
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused) HX_NOEXCEPT
		{
			return alloc(reused);
		}
	};

	template <typename T, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault, size_t BufMemAlignment = MemoryAlignment<T>::value>
	class SharedFixedSizeBufferPool1 : public SharedFixedSizeBufferPoolX<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>
	{
		using _MyBase = SharedFixedSizeBufferPoolX<T, BufMemGlobal, ObjMemGlobal>;

	public:
		//表示状态。
		using State = FixedSizePoolState;
		//对象内存池。
		using ObjMemPool = MemoryFixedSizePoolResource<ObjMemGlobal>;
		//缓存内存池。
		using BufMemPool = MemoryFixedSizePool<T, BufMemGlobal, BufMemAlignment>;

	private:
		//表示删除器
		struct Deleter
		{
			size_t size;
			::std::function<void()> func;
			::std::shared_ptr<BufMemPool> pool;

			//构造函数
			HX_INLINE Deleter(size_t size, const ::std::shared_ptr<BufMemPool>& pool) HX_NOEXCEPT
				: size(size), pool(pool)
			{
				//NOTHING
			}
			HX_INLINE Deleter(size_t size, const ::std::function<void()>& func, const ::std::shared_ptr<BufMemPool>& pool) HX_NOEXCEPT
				: size(size), func(func), pool(pool)
			{
				//NOTHING
			}
			HX_INLINE Deleter(size_t size, ::std::function<void()>&& func, const ::std::shared_ptr<BufMemPool>& pool) HX_NOEXCEPT
				: size(size), func(::std::move(func)), pool(pool)
			{
				//NOTHING
			}

			//执行函数
			HX_INLINE void operator()(T* ptr) HX_NOEXCEPT
			{
				if (func)
				{
					try
					{
						func();
					}
					catch (...)
					{
						//NOTHING
					}
				}
				pool->free(ptr, size);
			}
		};
		//表示内存分配器
		using MemAlloc = SharedBufferPoolAllocator<char, ObjMemPool>;

	public:
		//初始化缓存池对象。
		HX_INLINE SharedFixedSizeBufferPool1(size_t size = 1, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(size, limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedFixedSizeBufferPool1(SharedFixedSizeBufferPool1&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动操作符
		HX_INLINE SharedFixedSizeBufferPool1& operator=(SharedFixedSizeBufferPool1&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_objMemPool = ::std::move(other.m_objMemPool);
				_MyBase::m_bufMemPool = ::std::move(other.m_bufMemPool);
			}
			return *this;
		}

		//复制构造函数
		HX_INLINE SharedFixedSizeBufferPool1(const SharedFixedSizeBufferPool1& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//复制操作符
		HX_INLINE SharedFixedSizeBufferPool1& operator=(const SharedFixedSizeBufferPool1& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_objMemPool = other.m_objMemPool;
				_MyBase::m_bufMemPool = other.m_bufMemPool;
			}
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(SharedFixedSizeBufferPool1& other) HX_NOEXCEPT
		{
			_MyBase::swap(other);
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool& reused) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, &reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, nullptr);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, func, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, nullptr);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, ::std::move(func), _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, &reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, func, _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> alloc(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size;
			auto buf = _MyBase::m_bufMemPool->alloc(size, &reused);
			if (buf != nullptr)
			{
				try
				{
					MemAlloc alloc(_MyBase::m_objMemPool);
					return ::std::shared_ptr<T>(buf, Deleter(size, ::std::move(func), _MyBase::m_bufMemPool), alloc);
				}
				catch (...)//::std::bad_alloc
				{
					_MyBase::m_bufMemPool->free(buf, size);
				}
			}
			return ::std::shared_ptr<T>();
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool* reused = nullptr) HX_NOEXCEPT
		{
			return alloc(reused);
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused) HX_NOEXCEPT
		{
			return alloc(reused);
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			return alloc(func);
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(::std::function<void()>&& func) HX_NOEXCEPT
		{
			return alloc(::std::move(func));
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			return alloc(reused, func);
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			return alloc(reused, ::std::move(func));
		}
	};

	template <typename T, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault, size_t BufMemAlignment = MemoryAlignment<T>::value>
	using SharedFixedSizeBufferPool = SharedFixedSizeBufferPool1<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>;
}

template <typename T, typename BufMemGlobal = HxMemoryGlobalDefault, typename ObjMemGlobal = HxMemoryGlobalDefault, size_t BufMemAlignment = HxMemoryAlignment<T>::value>
using HxSharedFixedSizeBufferPool = ::HxLibs::SharedFixedSizeBufferPool1<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>;

template <typename T, typename BufMemGlobal = HxMemoryGlobalDefault, typename ObjMemGlobal = HxMemoryGlobalDefault, size_t BufMemAlignment = HxMemoryAlignment<T>::value>
using HxSharedFixedSizeBufferPool0 = ::HxLibs::SharedFixedSizeBufferPool0<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>;

template <typename T, typename BufMemGlobal = HxMemoryGlobalDefault, typename ObjMemGlobal = HxMemoryGlobalDefault, size_t BufMemAlignment = HxMemoryAlignment<T>::value>
using HxSharedFixedSizeBufferPool1 = ::HxLibs::SharedFixedSizeBufferPool1<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>;

#endif //__cplusplus

#endif //!HX_SHARED_FIXEDSIZE_BUFFER_POOL

#endif // !HX_CORE_SHARED_FIXEDSIZE_BUFFER_POOL_H
