/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SHARED_OBJECT_POOL_H
#define HX_CORE_SHARED_OBJECT_POOL_H

#ifndef HX_SHARED_OBJECT_POOL
#define HX_SHARED_OBJECT_POOL

#include "FixedSizePoolFlags.h"
#include "FixedSizePoolState.h"

HX_STRUCT_ALIAS(HxFixedSizePoolState, HxSharedObjectPoolState);

#ifdef __cplusplus
#include "Memory.h"
#include "SharedBufferPoolAllocator.h"
#include "MemoryResourceAllocator.h"
#include "MemoryFixedSizePoolResource.h"

static HX_CONSTEXPR int HX_SHARED_OBJECT_POOL_LIMIT_NONE = HX_FIXEDSIZE_POOL_LIMIT_NONE;
static HX_CONSTEXPR int HX_SHARED_OBJECT_POOL_LIMIT_RESERVED = HX_FIXEDSIZE_POOL_LIMIT_RESERVED;
static HX_CONSTEXPR int HX_SHARED_OBJECT_POOL_CACHE_RESERVED = HX_FIXEDSIZE_POOL_CACHE_RESERVED;

namespace HxLibs
{
	using SharedObjectPoolState = FixedSizePoolState;

	static HX_CONSTEXPR int SHARED_OBJECT_POOL_LIMIT_NONE = HX_SHARED_OBJECT_POOL_LIMIT_NONE;
	static HX_CONSTEXPR int SHARED_OBJECT_POOL_LIMIT_RESERVED = HX_SHARED_OBJECT_POOL_LIMIT_RESERVED;
	static HX_CONSTEXPR int SHARED_OBJECT_POOL_CACHE_RESERVED = HX_SHARED_OBJECT_POOL_CACHE_RESERVED;


	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class SharedObjectPoolX
	{
	public:
		//表示状态。
		using State = SharedObjectPoolState;
		//表示缓存池。
		using Pool = MemoryFixedSizePoolResource<MemGlobal>;

	protected:
		//缓存池
		::std::shared_ptr<Pool> m_pool;

		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> allocate(const Alloc& alloc, TArgs&&... args) HX_NOEXCEPT
		{
			try
			{
				return ::std::allocate_shared<T, Alloc, TArgs...>(alloc, ::std::forward<TArgs>(args)...);
			}
			catch (...)//::std::bad_alloc
			{
				return ::std::shared_ptr<T>();
			}
		}

		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPoolX(::std::shared_ptr<Pool>&& pool) HX_NOEXCEPT
			: m_pool(::std::move(pool))
		{
			//NOTHING
		}
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPoolX(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: m_pool(pool)
		{
			//NOTHING
		}

		//初始化缓存池对象。
		HX_INLINE SharedObjectPoolX(int limit = HX_SHARED_OBJECT_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_pool(Memory::makeSharedObject<Pool>(limit, cache))
		{
			//NOTHING
		}

		//释放当前对象所占用的相关资源。
		HX_INLINE ~SharedObjectPoolX() HX_NOEXCEPT
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedObjectPoolX(SharedObjectPoolX&& other) HX_NOEXCEPT
			: m_pool(::std::move(other.m_pool))
		{
			//NOTHING
		}
		//复制构造函数
		HX_INLINE SharedObjectPoolX(const SharedObjectPoolX& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING
		}

		//交换对象池。
		HX_INLINE void swap(SharedObjectPoolX& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_pool, other.m_pool);
			}
		}

	public:
		//获取内存池。
		HX_INLINE explicit operator ::std::shared_ptr<Pool>& () HX_NOEXCEPT
		{
			return m_pool;
		}
		HX_INLINE explicit operator const ::std::shared_ptr<Pool>& () const HX_NOEXCEPT
		{
			return m_pool;
		}

		//获取缓存上限。
		HX_INLINE int limit() const HX_NOEXCEPT
		{
			return m_pool->limit();
		}
		//获取可缓存量。
		HX_INLINE int cache() const HX_NOEXCEPT
		{
			return m_pool->cache();
		}
		//获取活动数量。
		HX_INLINE int active() const HX_NOEXCEPT
		{
			return m_pool->active();
		}
		//获取缓存数量。
		HX_INLINE int cached() const HX_NOEXCEPT
		{
			return m_pool->cached();
		}

		//获取使用率（0.0 - 1.0）。
		HX_INLINE double usage() const HX_NOEXCEPT
		{
			return m_pool->usage();
		}

		//重置缓存上限和缓存数量。
		HX_INLINE void reset(int limit, int cache) HX_NOEXCEPT
		{
			m_pool->reset(limit, cache);
		}

		//查询当前状态。
		HX_INLINE void query(State* state) const HX_NOEXCEPT
		{
			m_pool->query(state);
		}
		//查询当前信息。
		HX_INLINE void query(int* limit, int* cache) const HX_NOEXCEPT
		{
			m_pool->query(limit, cache);
		}

		//设置缓存上限。
		HX_INLINE void setLimit(int value) HX_NOEXCEPT
		{
			m_pool->setLimit(value);
		}
		//设置可缓存数。
		HX_INLINE void setCache(int value) HX_NOEXCEPT
		{
			m_pool->setCache(value);
		}
	};


	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class SharedObjectPool0 : public SharedObjectPoolX<T, MemGlobal>
	{
		using _MyBase = SharedObjectPoolX<T, MemGlobal>;

	public:
		//表示状态。
		using State = SharedObjectPoolState;
		//表示缓存池。
		using Pool = MemoryFixedSizePoolResource<MemGlobal>;
		//表示分配类。
		template <typename T2>
		using Alloc = SharedBufferPoolAllocator0<T2, Pool>;

	public:
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool0(::std::shared_ptr<Pool>&& pool) HX_NOEXCEPT
			: _MyBase(::std::move(pool))
		{
			//NOTHING
		}
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool0(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: _MyBase(pool)
		{
			//NOTHING
		}

		//初始化缓存池对象。
		HX_INLINE SharedObjectPool0(int limit = HX_SHARED_OBJECT_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedObjectPool0(SharedObjectPool0&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE SharedObjectPool0& operator=(SharedObjectPool0&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE SharedObjectPool0(const SharedObjectPool0& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE SharedObjectPool0& operator=(const SharedObjectPool0& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = other.m_pool;
			}
			return *this;
		}

		//交换对象池。
		HX_INLINE void swap(SharedObjectPool0& other) HX_NOEXCEPT
		{
			_MyBase::swap(other);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
	};


	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class SharedObjectPool1_1 : public SharedObjectPoolX<T, MemGlobal>
	{
		using _MyBase = SharedObjectPoolX<T, MemGlobal>;

	public:
		//表示状态。
		using State = SharedObjectPoolState;
		//表示缓存池。
		using Pool = MemoryFixedSizePoolResource<MemGlobal>;
		//表示分配类。
		template <typename T1>
		using Alloc = SharedBufferPoolAllocator1_1<T1, Pool>;

	public:
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool1_1(::std::shared_ptr<Pool>&& pool) HX_NOEXCEPT
			: _MyBase(::std::move(pool))
		{
			//NOTHING
		}
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool1_1(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: _MyBase(pool)
		{
			//NOTHING
		}

		//初始化缓存池对象。
		HX_INLINE SharedObjectPool1_1(int limit = HX_SHARED_OBJECT_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedObjectPool1_1(SharedObjectPool1_1&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE SharedObjectPool1_1& operator=(SharedObjectPool1_1&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE SharedObjectPool1_1(const SharedObjectPool1_1& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE SharedObjectPool1_1& operator=(const SharedObjectPool1_1& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = other.m_pool;
			}
			return *this;
		}

		//交换对象池。
		HX_INLINE void swap(SharedObjectPool1_1& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::swap(other);
			}
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1_1(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func));
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1_1(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func));
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
	};

	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class SharedObjectPool1_2 : public SharedObjectPoolX<T, MemGlobal>
	{
		using _MyBase = SharedObjectPoolX<T, MemGlobal>;

	public:
		//表示状态。
		using State = SharedObjectPoolState;
		//表示缓存池。
		using Pool = MemoryFixedSizePoolResource<MemGlobal>;
		//表示分配类。
		template <typename T1>
		using Alloc = SharedBufferPoolAllocator1_2<T1, Pool>;

	public:
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool1_2(::std::shared_ptr<Pool>&& pool) HX_NOEXCEPT
			: _MyBase(::std::move(pool))
		{
			//NOTHING
		}
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool1_2(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: _MyBase(pool)
		{
			//NOTHING
		}

		//初始化缓存池对象。
		HX_INLINE SharedObjectPool1_2(int limit = HX_SHARED_OBJECT_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedObjectPool1_2(SharedObjectPool1_2&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE SharedObjectPool1_2& operator=(SharedObjectPool1_2&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE SharedObjectPool1_2(const SharedObjectPool1_2& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE SharedObjectPool1_2& operator=(const SharedObjectPool1_2& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = other.m_pool;
			}
			return *this;
		}

		//交换对象池。
		HX_INLINE void swap(SharedObjectPool1_2& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::swap(other);
			}
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1_1(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1_1(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
	};

	template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
	using SharedObjectPool1 = ::HxLibs::SharedObjectPool1_1<T, MemGlobal>;


	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class SharedObjectPool2 : public SharedObjectPoolX<T, MemGlobal>
	{
		using _MyBase = SharedObjectPoolX<T, MemGlobal>;

	public:
		//表示状态。
		using State = SharedObjectPoolState;
		//表示缓存池。
		using Pool = MemoryFixedSizePoolResource<MemGlobal>;
		//表示分配类。
		template <typename T2>
		using Alloc = SharedBufferPoolAllocator2<T2, Pool>;

	public:
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool2(::std::shared_ptr<Pool>&& pool) HX_NOEXCEPT
			: _MyBase(::std::move(pool))
		{
			//NOTHING
		}
		//初始化缓存池对象。
		HX_INLINE explicit SharedObjectPool2(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: _MyBase(pool)
		{
			//NOTHING
		}

		//初始化缓存池对象。
		HX_INLINE SharedObjectPool2(int limit = HX_SHARED_OBJECT_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: _MyBase(limit, cache)
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE SharedObjectPool2(SharedObjectPool2&& other) HX_NOEXCEPT
			: _MyBase(::std::move(other))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE SharedObjectPool2& operator=(SharedObjectPool2&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE SharedObjectPool2(const SharedObjectPool2& other) HX_NOEXCEPT
			: _MyBase(other)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE SharedObjectPool2& operator=(const SharedObjectPool2& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				_MyBase::m_pool = other.m_pool;
			}
			return *this;
		}

		//交换对象池。
		HX_INLINE void swap(SharedObjectPool2& other) HX_NOEXCEPT
		{
			_MyBase::swap(other);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc0(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc1(::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func));
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc2(bool& reused, const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> alloc2(bool& reused, ::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func), &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share0(TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func));
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(bool& reused, const ::std::function<void()>& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, func, &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(bool& reused, ::std::function<void()>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			Alloc<T> alloc(_MyBase::m_pool, ::std::move(func), &reused);
			return _MyBase::allocate(alloc, ::std::forward<TArgs>(args)...);
		}
	};


	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	using SharedObjectPool = SharedObjectPool2<T, MemGlobal>;
}

template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool = ::HxLibs::SharedObjectPool2<T, MemGlobal>;

template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool0 = ::HxLibs::SharedObjectPool0<T, MemGlobal>;

template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool1 = ::HxLibs::SharedObjectPool1<T, MemGlobal>;
template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool1_1 = ::HxLibs::SharedObjectPool1_1<T, MemGlobal>;
template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool1_2 = ::HxLibs::SharedObjectPool1_2<T, MemGlobal>;

template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxSharedObjectPool2 = ::HxLibs::SharedObjectPool2<T, MemGlobal>;

#else //!__cplusplus

#define HX_SHARED_OBJECT_POOL_LIMIT_NONE HX_FIXEDSIZE_POOL_LIMIT_NONE
#define HX_SHARED_OBJECT_POOL_LIMIT_RESERVED HX_FIXEDSIZE_POOL_LIMIT_RESERVED
#define HX_SHARED_OBJECT_POOL_CACHE_RESERVED HX_FIXEDSIZE_POOL_CACHE_RESERVED

#endif //__cplusplus

#endif //!HX_SHARED_OBJECT_POOL

#endif // !HX_CORE_SHARED_OBJECT_POOL_H
