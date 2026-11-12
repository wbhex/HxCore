/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_OBJECT_POOL_H
#define HX_CORE_OBJECT_POOL_H

#ifdef __cplusplus
#ifndef HX_OBJECT_POOL
#define HX_OBJECT_POOL

#include <memory>
#include <functional>
#include "RefCnt.h"
#include "Reference.h"
#include "Memory.h"
#include "MemoryGlobalDefault.h"
#include "MemoryResourceAllocator.h"
#include "MemoryFixedSizePoolResource.h"
#include "SharedBufferPoolAllocator.h"
#include "MemoryFixedSizePool.h"

namespace HxLibs
{
	template <typename T, typename MemGlobal = MemoryGlobalDefault>
	class ObjectPool
	{
	public:
		//表示缓存池状态。
		using State = FixedSizePoolState;

	private:
		//表示缓存池。
		template <typename Object>
		class Pool
		{
		private:
			MemoryFixedSizePool<unsigned char, MemGlobal> m_bufMemPool;
			::std::shared_ptr<MemoryFixedSizePoolResource<MemGlobal>> m_sharedMemPool;

		public:
			//初始化缓存池。
			HX_INLINE Pool(int limit, int cache) HX_NOEXCEPT
				: m_bufMemPool(sizeof(T) + sizeof(Object), limit, cache)
				, m_sharedMemPool(Memory::makeSharedObject<MemoryFixedSizePoolResource<MemGlobal>>(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache))
			{
				//NOTHING
			}
			//释放当前对象所占用的相关资源。
			HX_INLINE ~Pool() HX_NOEXCEPT
			{
				//NOTHING
			}

			//获取缓存上限。
			HX_INLINE int limit() const HX_NOEXCEPT
			{
				return m_bufMemPool.limit();
			}
			//获取可缓存量。
			HX_INLINE int cache() const HX_NOEXCEPT
			{
				return m_bufMemPool.cache();
			}
			//获取活动数量。
			HX_INLINE int active() const HX_NOEXCEPT
			{
				return m_bufMemPool.active();
			}
			//获取缓存数量。
			HX_INLINE int cached() const HX_NOEXCEPT
			{
				return m_bufMemPool.cached();
			}

			//获取使用率（0.0 - 1.0）。
			HX_INLINE double usage() const HX_NOEXCEPT
			{
				return m_bufMemPool.usage();
			}

			//释放缓存内容。
			HX_INLINE void free(unsigned char* buf) HX_NOEXCEPT
			{
				m_bufMemPool.free(buf, sizeof(T) + sizeof(Object));
			}
			//分配缓存内容。
			HX_INLINE unsigned char* alloc(bool* reused) HX_NOEXCEPT
			{
				return m_bufMemPool.alloc(reused);
			}

			//查询当前状态。
			HX_INLINE void query(State& state) const HX_NOEXCEPT
			{
				m_bufMemPool.query(state);
				state.size = sizeof(T);
			}
			//查询当前信息。
			HX_INLINE void query(int* limit, int* cache) const HX_NOEXCEPT
			{
				m_bufMemPool.query(nullptr, limit, cache);
			}

			//重置缓存上限和缓存数量。
			HX_INLINE void reset(int limit, int cache) HX_NOEXCEPT
			{
				m_bufMemPool.reset(HX_FIXEDSIZE_POOL_SIZE_RESERVED, limit, cache);
				m_sharedMemPool->reset(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache);
			}

			//设置缓存上限。
			HX_INLINE void setLimit(int value) HX_NOEXCEPT
			{
				m_bufMemPool.setLimit(value);
			}
			//设置可缓存数。
			HX_INLINE void setCache(int value) HX_NOEXCEPT
			{
				m_bufMemPool.setCache(value);
				m_sharedMemPool->setCache(value);
			}

			//共享内存池
			HX_INLINE::std::shared_ptr<MemoryFixedSizePoolResource<MemGlobal>>& sharedMemPool() HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
			HX_INLINE const ::std::shared_ptr<MemoryFixedSizePoolResource<MemGlobal>>& sharedMemPool() const HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
		};
		//表示对象类。
		class Entry : public Referencable
		{
		private:
			T* m_value;
			RefCnt m_refCnt;
			::std::shared_ptr<Pool<Entry>> m_pool;
			::std::function<void(T*)> m_func;

		public:
			//初始化对象。
			HX_INLINE Entry(T* value, const ::std::shared_ptr<Pool<Entry>>& pool, const ::std::function<void(T*)>& func) HX_NOEXCEPT
				: m_value(value), m_pool(pool), m_func(func)
			{
				//NOTHING
			}
			//初始化对象。
			HX_INLINE Entry(T* value, const ::std::shared_ptr<Pool<Entry>>& pool, ::std::function<void(T*)>&& func) HX_NOEXCEPT
				: m_value(value), m_pool(pool), m_func(::std::move(func))
			{
				//NOTHING
			}

			//释放当前对象所占用的相关资源。
			HX_INLINE ~Entry() HX_NOEXCEPT
			{
				m_value->~T();
			}

			//添加引用。
			HX_INLINE void HX_STDCALL addRef() HX_NOEXCEPT override
			{
				m_refCnt.addRef();
			}
			//释放引用，如果删除对象则返回True。
			HX_INLINE bool HX_STDCALL release() HX_NOEXCEPT override
			{
				if (m_refCnt.release())
				{
					if (m_func)
					{
						try
						{
							m_func(m_value);
						}
						catch (...)
						{
							//NOTHING
						}
					}
					::std::shared_ptr<Pool<Entry>> pool(::std::move(m_pool));
					{
						this->~Entry();
					}
					pool->free((unsigned char*)m_value - sizeof(Entry));
					return true;
				}
				return false;
			}

			//获取对象指针。
			HX_INLINE T* value() const HX_NOEXCEPT
			{
				return m_value;
			}

			//获取共享指针对象。
			HX_INLINE::std::shared_ptr<T> share() HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					SharedBufferPoolAllocator<T, MemoryFixedSizePoolResource<MemGlobal>> alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(m_value, [](T* p) { ((Entry*)((unsigned char*)p - sizeof(Entry)))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T>();
				}
			}
			//获取共享指针对象。
			template <typename Alloc>
			HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc) HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					return ::std::shared_ptr<T>(m_value, [](T* p) { ((Entry*)((unsigned char*)p - sizeof(Entry)))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T>();
				}
			}

			//获取转换为指定类型的共享指针对象。
			template <typename T2>
			HX_INLINE::std::shared_ptr<T2> shareAs() HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					SharedBufferPoolAllocator<T2, MemoryFixedSizePoolResource<MemGlobal>> alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T2>((T2*)m_value, [](T2* p) { ((Entry*)((unsigned char*)p - sizeof(Entry)))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T2>();
				}
			}
			//获取转换为指定类型的共享指针对象。
			template <typename T2, typename Alloc>
			HX_INLINE::std::shared_ptr<T2> shareAs(const Alloc& alloc) HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					return ::std::shared_ptr<T2>((T2*)m_value, [](T2* p) { ((Entry*)((unsigned char*)p - sizeof(Entry)))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T2>();
				}
			}
		};

	public:
		//表示对象引用。 
		class ObjRef
		{
		private:
			//内部指针。
			Entry* m_p;

		public:
			//初始化为指定指针对象引用。
			template <class... TArgs>
			HX_INLINE ObjRef(unsigned char* buf, const ::std::shared_ptr<Pool<Entry>>& pool, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
				: m_p(nullptr)
			{
				T* val = (T*)(buf + sizeof(Entry));
				try
				{
					val = new(val)T(::std::forward<TArgs>(args)...);
				}
				catch (...)
				{
					return;
				}
				m_p = new(buf)Entry(val, pool, func);
			}
			//初始化为指定指针对象引用。
			template <class... TArgs>
			HX_INLINE ObjRef(unsigned char* buf, const ::std::shared_ptr<Pool<Entry>>& pool, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
				: m_p(nullptr)
			{
				T* val = (T*)(buf + sizeof(Entry));
				try
				{
					val = new(val)T(::std::forward<TArgs>(args)...);
				}
				catch (...)
				{
					return;
				}
				m_p = new(buf)Entry(val, pool, ::std::move(func));
			}

			//初始为空引用。
			HX_INLINE ObjRef() HX_NOEXCEPT
				: m_p(nullptr)
			{
				//NOTHING
			}
			//初始化为空引用。
			HX_INLINE ObjRef(::std::nullptr_t) HX_NOEXCEPT
				: m_p(nullptr)
			{
				//NOTHING
			}
			//拷贝构造函数。
			HX_INLINE ObjRef(const ObjRef& other) HX_NOEXCEPT
				: m_p(other.m_p)
			{
				if (m_p != nullptr)
				{
					m_p->addRef();
				}
			}
			//移动构造函数。
			HX_INLINE ObjRef(ObjRef&& other) HX_NOEXCEPT
				: m_p(other.m_p)
			{
				other.m_p = nullptr;
			}

			//析构函数。
			HX_INLINE ~ObjRef() HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					m_p->release();
					m_p = nullptr;
				}
			}

			//判断是否有效。
			HX_INLINE operator bool() const HX_NOEXCEPT
			{
				return m_p != nullptr;
			}

			//获取指针对象。
			HX_INLINE T* operator->() const HX_NOEXCEPT
			{
				return m_p->value();
			}

			//交换两个对象。
			HX_INLINE void swap(ObjRef& other) const HX_NOEXCEPT
			{
				if (this != &other)
				{
					::std::swap(m_p, other.m_p);
				}
			}

			//获取共享指针对象。
			HX_INLINE::std::shared_ptr<T> share() HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					return m_p->share();
				}
				return ::std::shared_ptr<T>();
			}
			//获取共享指针对象。
			template <typename Alloc>
			HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc) HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					return m_p->share(alloc);
				}
				return ::std::shared_ptr<T>();
			}

			//获取共享指针对象。
			template <typename T2>
			HX_INLINE::std::shared_ptr<T2> shareAs() HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					return m_p->shareAs();
				}
				return ::std::shared_ptr<T2>();
			}
			//获取共享指针对象。
			template <typename T2, typename Alloc>
			HX_INLINE::std::shared_ptr<T2> shareAs(const Alloc& alloc) HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					return m_p->shareAs(alloc);
				}
				return ::std::shared_ptr<T2>();
			}

			//判断是否有效。
			HX_INLINE bool valid() const HX_NOEXCEPT
			{
				return m_p != nullptr;
			}
			//判断是否无效。
			HX_INLINE bool invalid() const HX_NOEXCEPT
			{
				return m_p == nullptr;
			}

			//获取目标对象。
			HX_INLINE T* target() const HX_NOEXCEPT
			{
				return m_p->value();
			}

			//获取引用对象。
			HX_INLINE Reference<T> reference() const HX_NOEXCEPT
			{
				return Reference<T>(m_p->value(), m_p);
			}

			//赋值操作符。
			HX_INLINE ObjRef& operator=(::std::nullptr_t) HX_NOEXCEPT
			{
				if (m_p != nullptr)
				{
					m_p->release();
					m_p = nullptr;
				}
				return *this;
			}
			//赋值操作符。
			HX_INLINE ObjRef& operator=(const ObjRef& other) HX_NOEXCEPT
			{
				if (m_p != other.m_p)
				{
					if (m_p != nullptr)
					{
						m_p->release();
					}
					m_p = other.m_p;
					if (m_p != nullptr)
					{
						m_p->addRef();
					}
				}
				return *this;
			}
			//移动操作符。
			HX_INLINE ObjRef& operator=(ObjRef&& other) HX_NOEXCEPT
			{
				if (m_p != other.m_p)
				{
					if (m_p != nullptr)
					{
						m_p->release();
					}
					m_p = other.m_p;
					other->m_p = nullptr;
				}
				else if (&other != this)
				{
					other->m_p = nullptr;
				}
				return *this;
			}

			//等于操作符。
			HX_INLINE bool operator==(::std::nullptr_t) const HX_NOEXCEPT
			{
				return m_p == nullptr;
			}
			//等于操作符。
			HX_INLINE bool operator==(const T* other) const HX_NOEXCEPT
			{
				if (m_p == nullptr)
				{
					return other == nullptr;
				}
				else
				{
					return m_p->value() == other;
				}
			}
			//等于操作符。
			HX_INLINE bool operator==(const ObjRef& other) const HX_NOEXCEPT
			{
				return m_p == other.m_p;
			}

			//不等于操作符。
			HX_INLINE bool operator!=(::std::nullptr_t) const HX_NOEXCEPT
			{
				return m_p != nullptr;
			}
			//不等于操作符。
			HX_INLINE bool operator!=(const T* other) const HX_NOEXCEPT
			{
				if (m_p == nullptr)
				{
					return other != nullptr;
				}
				else
				{
					return m_p->value() != other;
				}
			}
			//不等于操作符。
			HX_INLINE bool operator!=(const ObjRef& other) const HX_NOEXCEPT
			{
				return m_p != other.m_p;
			}
		};

	private:
		//缓存池
		::std::shared_ptr<Pool<Entry>> m_pool;

	public:
		//初始化缓存池对象。
		HX_INLINE ObjectPool(int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_pool(Memory::makeSharedObject<Pool<Entry>>(limit, cache))
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		HX_INLINE ~ObjectPool() HX_NOEXCEPT
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE ObjectPool(ObjectPool&& other) HX_NOEXCEPT
			: m_pool(::std::move(other.m_pool))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE ObjectPool& operator=(ObjectPool&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE ObjectPool(const ObjectPool& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING 
		}
		//拷贝赋值操作符
		HX_INLINE ObjectPool& operator=(const ObjectPool& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = other.m_pool;
			}
			return *this;
		}

		//交换对象池。
		HX_INLINE void swap(ObjectPool& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_pool, other.m_pool);
			}
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

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc(bool* reused, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(reused);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, func, ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc(bool* reused, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(reused);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, ::std::move(func), ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc0(TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(nullptr);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, ::std::move(::std::function<void(T*)>()), ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(&reused);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, ::std::move(::std::function<void(T*)>()), ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc1(const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(nullptr);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, func, ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc1(::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(nullptr);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, ::std::move(func), ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc2(bool& reused, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(&reused);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, func, ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef alloc2(bool& reused, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			unsigned char* buf = m_pool->alloc(&reused);
			if (buf != nullptr)
			{
				ObjRef value(buf, m_pool, ::std::move(func), ::std::forward<TArgs>(args)...);
				if (!value.valid())
				{
					m_pool->free(buf);
				}
				return value;
			}
			return ObjRef();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef share(bool* reused, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc(reused, func, ::std::forward<TArgs>(args)...).share();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE ObjRef share(bool* reused, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc(reused, ::std::move(func), ::std::forward<TArgs>(args)...).share();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share0(TArgs&&... args) HX_NOEXCEPT
		{
			return alloc0(::std::forward<TArgs>(args)...).share();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(reused, ::std::forward<TArgs>(args)...).share();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(func, ::std::forward<TArgs>(args)...).share();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(::std::move(func), ::std::forward<TArgs>(args)...).share();
		}

		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(bool& reused, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc2(reused, func, ::std::forward<TArgs>(args)...).share();
		}
		//获取一个缓存对象。
		template <class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(bool& reused, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc2(reused, ::std::move(func), ::std::forward<TArgs>(args)...).share();
		}

		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share1(const Alloc& alloc, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc0(::std::forward<TArgs>(args)...).share(alloc);
		}

		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(const Alloc& alloc, bool& reused, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(reused, ::std::forward<TArgs>(args)...).share(alloc);
		}
		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(const Alloc& alloc, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(func, ::std::forward<TArgs>(args)...).share(alloc);
		}
		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share2(const Alloc& alloc, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc1(::std::move(func), ::std::forward<TArgs>(args)...).share(alloc);
		}

		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share3(const Alloc& alloc, bool& reused, const ::std::function<void(T*)>& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc2(reused, func, ::std::forward<TArgs>(args)...).share(alloc);
		}
		//获取一个缓存对象。
		template <typename Alloc, class... TArgs>
		HX_INLINE::std::shared_ptr<T> share3(const Alloc& alloc, bool& reused, ::std::function<void(T*)>&& func, TArgs&&... args) HX_NOEXCEPT
		{
			return alloc2(reused, ::std::move(func), ::std::forward<TArgs>(args)...).share(alloc);
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

	template <typename T, typename MemGlobal>
	HX_INLINE bool operator==(::std::nullptr_t, const typename ObjectPool<T, MemGlobal>::ObjRef& value) HX_NOEXCEPT
	{
		return value == nullptr;
	}
	template <typename T, typename MemGlobal>
	HX_INLINE bool operator==(const T* left, const typename ObjectPool<T, MemGlobal>::ObjRef& right) HX_NOEXCEPT
	{
		return right == left;
	}

	template <typename T, typename MemGlobal>
	HX_INLINE bool operator!=(::std::nullptr_t, const typename ObjectPool<T, MemGlobal>::ObjRef& value) HX_NOEXCEPT
	{
		return value != nullptr;
	}
	template <typename T, typename MemGlobal>
	HX_INLINE bool operator!=(const T* left, const typename ObjectPool<T, MemGlobal>::ObjRef& right) HX_NOEXCEPT
	{
		return right != left;
	}
}

template <typename T, typename MemGlobal = HxMemoryGlobalDefault>
using HxObjectPool = ::HxLibs::ObjectPool<T, MemGlobal>;

#endif //!HX_OBJECT_POOL
#endif //__cplusplus

#endif // !HX_CORE_OBJECT_POOL_H
