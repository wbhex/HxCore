/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_FIXEDSIZE_BUFFER_POOL_H
#define HX_CORE_FIXEDSIZE_BUFFER_POOL_H

#ifdef __cplusplus
#ifndef HX_FIXEDSIZE_BUFFER_POOL
#define HX_FIXEDSIZE_BUFFER_POOL

#include <functional>

#include "Memory.h"
#include "RefCnt.h"
#include "ObjRef.h"
#include "BufView.h"
#include "Referencable.h"
#include "MemoryFixedSizePool.h"
#include "MemoryGlobalDefault.h"
#include "SharedBufferPoolAllocator.h"
#include "MemoryFixedSizePoolResource.h"
#include "FixedSizePoolFlags.h"
#include "FixedSizePoolState.h"

namespace HxLibs
{
	template <typename T, typename MemGlobal = MemoryGlobalDefault, size_t MemAlignment = MemoryAlignment<T>::value>
	class FixedSizeBufferPool
	{
	public:
		//表示状态。
		using State = HxFixedSizePoolState;

	private:
		template <typename TObj>
		struct ObjSize
		{
			static HX_CONSTEXPR size_t value = (sizeof(TObj) + MemAlignment - 1) / MemAlignment * MemAlignment;
		};

		using BufMemPool = MemoryFixedSizePool<unsigned char, MemGlobal, MemAlignment>;
		using SharedMemPool = MemoryFixedSizePoolResource<MemGlobal>;
		using SharedMemAlloc = SharedBufferPoolAllocator<T, SharedMemPool>;

		//表示缓存池。
		template <typename Object>
		class Pool
		{
		private:
			size_t m_size;
			size_t m_bytes;
			BufMemPool m_bufMemPool;
			::std::shared_ptr<SharedMemPool> m_sharedMemPool;

		public: 
			//初始化缓存池。
			HX_INLINE Pool(size_t size, int limit, int cache) HX_NOEXCEPT
				: m_size(size)
				, m_bytes(ByteLength<T>::count(size))
				, m_bufMemPool(m_bytes + ObjSize<Object>::value, limit, cache)
				, m_sharedMemPool(Memory::makeSharedObject<SharedMemPool>(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache))
			{
				//NOTHING
			}
			//释放当前对象所占用的相关资源。
			HX_INLINE ~Pool() HX_NOEXCEPT
			{
				//NOTHING
			}

			//获取缓存大小。
			HX_INLINE size_t size() const HX_NOEXCEPT
			{
				return m_size;
			}
			//获取缓存字节大小。
			HX_INLINE size_t bytes() const HX_NOEXCEPT
			{
				return m_bytes;
			}

			//获取缓存字节大小。
			HX_INLINE size_t bufSize() const HX_NOEXCEPT
			{
				return m_bufMemPool.size();
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
			HX_INLINE void free(unsigned char* buf, size_t size) HX_NOEXCEPT
			{
				m_bufMemPool.free(buf, ByteLength<T>::count(size) + ObjSize<Object>::value);
			}
			//分配缓存内容。
			HX_INLINE unsigned char* alloc(size_t& size, size_t& bytes, bool* reused) HX_NOEXCEPT
			{
				size_t size2;
				auto buf = m_bufMemPool.alloc(size2, reused);
				if (buf)
				{
					bytes = size2 - ObjSize<Object>::value;
					size = bytes / Sizeof<T>::value;
					return (unsigned char*)buf;
				}
				return nullptr;
			}

			//查询当前状态。
			HX_INLINE void query(State& state) const HX_NOEXCEPT
			{
				m_bufMemPool.query(state);
				state.size = (state.size - ObjSize<Object>::value) / Sizeof<T>::value;
			}
			//查询当前信息。
			HX_INLINE void query(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
			{
				m_bufMemPool.query(size, limit, cache);
				if (size != nullptr)
				{
					*size = ((*size) - ObjSize<Object>::value) / sizeof(T);
				}
			}

			//重置缓存池。
			HX_INLINE void reset(size_t size, int limit, int cache) HX_NOEXCEPT
			{
				if (size != HX_FIXEDSIZE_POOL_SIZE_RESERVED)
				{
					auto bytes = ByteLength<T>::count(size);
					m_size = size;
					m_bytes = bytes;
					m_bufMemPool.reset(bytes + ObjSize<Object>::value, limit, cache);
				}
				else
				{
					m_bufMemPool.reset(HX_FIXEDSIZE_POOL_SIZE_RESERVED, limit, cache);
				}
				m_sharedMemPool->reset(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache);
			}

			//设置缓存大小。
			HX_INLINE void setSize(size_t value) HX_NOEXCEPT
			{
				auto bytes = ByteLength<T>::count(value);
				m_size = value;
				m_bytes = bytes;
				m_bufMemPool.setSize(bytes + ObjSize<Object>::value);
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
			HX_INLINE::std::shared_ptr<SharedMemPool>& sharedMemPool() HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
			HX_INLINE const ::std::shared_ptr<SharedMemPool>& sharedMemPool() const HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
		};

	public:
		//表示简单缓存对象。
		class Buffer : public Referencable, Uncopyable
		{
			friend class ::HxLibs::FixedSizeBufferPool<T, MemGlobal>;

		private:
			T* m_data;
			size_t m_size;
			size_t m_bytes;
			RefCnt m_refCnt;
			::std::shared_ptr<Pool<Buffer>> m_pool;
			::std::function<void()> m_func;

			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool)
			{
				//NOTHING
			}
			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool, const ::std::function<void()>& func) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool), m_func(func)
			{
				//NOTHING
			}
			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool, ::std::function<void()>&& func) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool), m_func(::std::move(func))
			{
				//NOTHING
			}

			//释放当前对象所占用的相关资源。
			HX_INLINE ~Buffer() HX_NOEXCEPT
			{
				//NOTHING
			}

		public:
			HX_INLINE T* ptr() HX_NOEXCEPT
			{
				return m_data;
			}
			HX_INLINE const T* ptr() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取元素数据。
			HX_INLINE T* data() HX_NOEXCEPT
			{
				return m_data;
			}
			//获取元素数据。
			HX_INLINE const T* data() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取元素数量。
			HX_INLINE size_t size() const HX_NOEXCEPT
			{
				return m_size;
			}

			//获取指定索引的元素。
			HX_INLINE T& at(size_t i) HX_NOEXCEPT
			{
				return m_data[i];
			}
			//获取指定索引的元素。
			HX_INLINE const T& at(size_t i) const HX_NOEXCEPT
			{
				return m_data[i];
			}

			//获取指定索引的元素。
			HX_INLINE T& operator[](size_t i) HX_NOEXCEPT
			{
				return m_data[i];
			}
			//获取指定索引的元素。
			HX_INLINE const T& operator[](size_t i) const HX_NOEXCEPT
			{
				return m_data[i];
			}

			//获取起始地址。
			HX_INLINE T* begin() HX_NOEXCEPT
			{
				return m_data;
			}
			//获取起始地址。
			HX_INLINE const T* begin() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取结束地址（不包含）。
			HX_INLINE T* end() HX_NOEXCEPT
			{
				return m_data + m_size;
			}
			//获取结束地址（不包含）。
			HX_INLINE const T* end() const HX_NOEXCEPT
			{
				return m_data + m_size;
			}

			//获取字节长度。
			HX_INLINE size_t bytes() const HX_NOEXCEPT
			{
				return m_bytes;
			}

			//添加引用。
			virtual void HX_STDCALL addRef() HX_NOEXCEPT override
			{
				m_refCnt.addRef();
			}
			//释放引用，如果删除对象则返回True。
			virtual bool HX_STDCALL release() HX_NOEXCEPT override
			{
				if (m_refCnt.release())
				{
					if (m_func)
					{
						try
						{
							m_func();
						}
						catch (...)
						{
							//NOTHING
						}
					}
					::std::shared_ptr<Pool<Buffer>> pool(::std::move(m_pool));
					{
						this->~Buffer();
					}
					pool->free((unsigned char*)m_data - ObjSize<Buffer>::value, m_size);
					return true;
				}
				return false;
			}

			//获取共享指针对象。
			HX_INLINE::std::shared_ptr<T> share() HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(m_data, [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
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
					return ::std::shared_ptr<T>(m_data, [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
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
					return ::std::shared_ptr<T2>((T2*)m_data, [](T2* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
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
					return ::std::shared_ptr<T2>((T2*)m_data, [](T2* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T2>();
				}
			}
		};

	private:
		//缓存池
		::std::shared_ptr<Pool<Buffer>> m_pool;

	public:
		//初始化缓存池对象。
		HX_INLINE FixedSizeBufferPool(size_t size = 1, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_pool(Memory::makeSharedObject<Pool<Buffer>>(size, limit, cache < 0 ? 0 : cache))
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		HX_INLINE ~FixedSizeBufferPool() HX_NOEXCEPT
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE FixedSizeBufferPool(FixedSizeBufferPool&& other) HX_NOEXCEPT
			: m_pool(::std::move(other.m_pool))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE FixedSizeBufferPool& operator=(FixedSizeBufferPool&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE FixedSizeBufferPool(const FixedSizeBufferPool& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE FixedSizeBufferPool& operator=(const FixedSizeBufferPool& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = other.m_pool;
			}
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(FixedSizeBufferPool& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool.swap(other.m_pool);
			}
		}

		//获取缓存大小。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_pool->size();
		}
		//获取缓存字节大小。
		HX_INLINE size_t bytes() const HX_NOEXCEPT
		{
			return m_pool->bytes();
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

		//重置缓存。
		HX_INLINE void reset(size_t size, int limit = HX_FIXEDSIZE_POOL_LIMIT_RESERVED, int cache = HX_FIXEDSIZE_POOL_CACHE_RESERVED) HX_NOEXCEPT
		{
			m_pool->reset(size, limit, cache);
		}

		//获取一个缓存对象。
		HX_INLINE bool alloc(BufView<T>* bufView, Referencable** bufItem, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, reused);
			if (buf != nullptr)
			{
				auto buffer = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				*bufItem = buffer;
				bufView->reset(buffer->ptr(), buffer->size());
				return true;
			}
			*bufItem = nullptr;
			return false;
		}
		//获取一个缓存对象。
		HX_INLINE bool alloc(BufView<T>& bufView, Referencable** bufItem, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, reused);
			if (buf != nullptr)
			{
				auto buffer = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				*bufItem = buffer;
				bufView.reset(buffer->ptr(), buffer->size());
				return true;
			}
			*bufItem = nullptr;
			return false;
		}

		//获取一个缓存对象。
		HX_INLINE Buffer* alloc() HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				return new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
			}
			return nullptr;
		}

		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool* reused = nullptr) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, reused);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, &reused);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(::std::function<void()>&& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func)));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, &reused);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes;
				unsigned char* buf = m_pool->alloc(size, bytes, &reused);
				if (buf != nullptr)
				{
					value.attach(new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func)));
				}
			}
			return value;
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}

		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, func);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes;
			unsigned char* buf = m_pool->alloc(size, bytes, &reused);
			if (buf != nullptr)
			{
				auto obj = new(buf)Buffer((T*)(buf + ObjSize<Buffer>::value), size, bytes, m_pool, ::std::move(func));
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [](T* p) { ((Buffer*)((unsigned char*)p - ObjSize<Buffer>::value))->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}

		//查询当前信息。
		HX_INLINE void queryInfo(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
		{
			m_pool->query(size, limit, cache);
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
				m_pool->query(*state);
			}
		}
		//查询当前状态。
		HX_INLINE void queryState(State& state) const HX_NOEXCEPT
		{
			m_pool->query(state);
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
				m_pool->setSize(value);
			}
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

	template <typename T, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault, size_t BufMemAlignment = MemoryAlignment<T>::value>
	class FixedSizeBufferPool2
	{
	public:
		//表示状态。
		using State = HxFixedSizePoolState;

	private:
		using BufMemPool = MemoryFixedSizePool<T, BufMemGlobal, BufMemAlignment>;
		using ObjMemPool = MemoryFixedSizePool<unsigned char, ObjMemGlobal>;
		using SharedMemPool = MemoryFixedSizePoolResource<ObjMemGlobal>;
		using SharedMemAlloc = SharedBufferPoolAllocator<T, SharedMemPool>;

		//表示缓存池。
		template <typename Object>
		class Pool
		{
		private:
			BufMemPool m_bufMemPool;
			ObjMemPool m_objMemPool;
			::std::shared_ptr<SharedMemPool> m_sharedMemPool;

		public:
			//初始化缓存池。
			HX_INLINE Pool(size_t size, int limit, int cache) HX_NOEXCEPT
				: m_bufMemPool(size, limit, cache)
				, m_objMemPool(sizeof(Object), limit, cache)
				, m_sharedMemPool(Memory::makeSharedObject<SharedMemPool>(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache))
			{
				//NOTHING
			}
			//释放当前对象所占用的相关资源。
			HX_INLINE ~Pool() HX_NOEXCEPT
			{
				//NOTHING
			}

			//获取缓存大小。
			HX_INLINE size_t size() const HX_NOEXCEPT
			{
				return m_bufMemPool.size();
			}
			//获取缓存字节大小。
			HX_INLINE size_t bytes() const HX_NOEXCEPT
			{
				return m_bufMemPool.bytes();
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
			HX_INLINE void free(unsigned char* buf1, unsigned char* buf2, size_t size) HX_NOEXCEPT
			{
				m_bufMemPool.free((T*)buf1, size);
				m_objMemPool.free(buf2, sizeof(Object));
			}
			//分配缓存内容。
			HX_INLINE unsigned char* alloc(size_t& size, size_t& bytes, unsigned char** objBuf, bool* reused) HX_NOEXCEPT
			{
				auto buf = m_bufMemPool.alloc(size, bytes, reused);
				if (buf)
				{
					*objBuf = m_objMemPool.alloc();
					if (*objBuf != nullptr)
					{
						return (unsigned char*)buf;
					}
					m_bufMemPool.free(buf, size);
				}
				return nullptr;
			}

			//查询当前状态。
			HX_INLINE void query(State& state) const HX_NOEXCEPT
			{
				m_bufMemPool.query(state);
			}
			//查询当前信息。
			HX_INLINE void query(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
			{
				m_bufMemPool.query(size, limit, cache);
			}

			//重置缓存池。
			HX_INLINE void reset(size_t size, int limit, int cache) HX_NOEXCEPT
			{
				m_bufMemPool.reset(size, limit, cache);
				m_objMemPool.reset(HX_FIXEDSIZE_POOL_SIZE_RESERVED, limit, cache);
				m_sharedMemPool->reset(HX_FIXEDSIZE_POOL_LIMIT_NONE, cache);
			}

			//设置缓存大小。
			HX_INLINE void setSize(size_t value) HX_NOEXCEPT
			{
				m_bufMemPool.setSize(value);
			}
			//设置缓存上限。
			HX_INLINE void setLimit(int value) HX_NOEXCEPT
			{
				m_bufMemPool.setLimit(value);
				m_objMemPool.setLimit(value);
			}
			//设置可缓存数。
			HX_INLINE void setCache(int value) HX_NOEXCEPT
			{
				m_bufMemPool.setCache(value);
				m_objMemPool.setCache(value);
				m_sharedMemPool->setCache(value);
			}

			//共享内存池
			HX_INLINE::std::shared_ptr<SharedMemPool>& sharedMemPool() HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
			HX_INLINE const ::std::shared_ptr<SharedMemPool>& sharedMemPool() const HX_NOEXCEPT
			{
				return m_sharedMemPool;
			}
		};

	public:
		//表示简单缓存对象。
		class Buffer : public Referencable, Uncopyable
		{
			friend class ::HxLibs::FixedSizeBufferPool2<T, BufMemGlobal, ObjMemGlobal>;

		private:
			T* m_data;
			size_t m_size;
			size_t m_bytes;
			RefCnt m_refCnt;
			::std::shared_ptr<Pool<Buffer>> m_pool;
			::std::function<void()> m_func;

			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool)
			{
				//NOTHING
			}
			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool, const ::std::function<void()>& func) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool), m_func(func)
			{
				//NOTHING
			}
			//初始化缓存对象。
			HX_INLINE Buffer(T* data, size_t size, size_t bytes, const ::std::shared_ptr<Pool<Buffer>>& pool, ::std::function<void()>&& func) HX_NOEXCEPT
				: m_data(data), m_size(size), m_bytes(bytes), m_pool(pool), m_func(::std::move(func))
			{
				//NOTHING
			}

			//释放当前对象所占用的相关资源。
			HX_INLINE ~Buffer() HX_NOEXCEPT
			{
				//NOTHING
			}

		public:
			HX_INLINE T* ptr() HX_NOEXCEPT
			{
				return m_data;
			}
			HX_INLINE const T* ptr() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取元素数据。
			HX_INLINE T* data() HX_NOEXCEPT
			{
				return m_data;
			}
			//获取元素数据。
			HX_INLINE const T* data() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取元素数量。
			HX_INLINE size_t size() const HX_NOEXCEPT
			{
				return m_size;
			}

			//获取指定索引的元素。
			HX_INLINE T& at(size_t i) HX_NOEXCEPT
			{
				return m_data[i];
			}
			//获取指定索引的元素。
			HX_INLINE const T& at(size_t i) const HX_NOEXCEPT
			{
				return m_data[i];
			}

			//获取指定索引的元素。
			HX_INLINE T& operator[](size_t i) HX_NOEXCEPT
			{
				return m_data[i];
			}
			//获取指定索引的元素。
			HX_INLINE const T& operator[](size_t i) const HX_NOEXCEPT
			{
				return m_data[i];
			}

			//获取起始地址。
			HX_INLINE T* begin() HX_NOEXCEPT
			{
				return m_data;
			}
			//获取起始地址。
			HX_INLINE const T* begin() const HX_NOEXCEPT
			{
				return m_data;
			}

			//获取结束地址（不包含）。
			HX_INLINE T* end() HX_NOEXCEPT
			{
				return m_data + m_size;
			}
			//获取结束地址（不包含）。
			HX_INLINE const T* end() const HX_NOEXCEPT
			{
				return m_data + m_size;
			}

			//获取字节长度。
			HX_INLINE size_t bytes() const HX_NOEXCEPT
			{
				return m_bytes;
			}

			//添加引用。
			virtual void HX_STDCALL addRef() HX_NOEXCEPT override
			{
				m_refCnt.addRef();
			}
			//释放引用，如果删除对象则返回True。
			virtual bool HX_STDCALL release() HX_NOEXCEPT override
			{
				if (m_refCnt.release())
				{
					if (m_func)
					{
						try
						{
							m_func();
						}
						catch (...)
						{
							//NOTHING
						}
					}
					::std::shared_ptr<Pool<Buffer>> pool(::std::move(m_pool));
					{
						this->~Buffer();
					}
					pool->free((unsigned char*)m_data, (unsigned char*)this, m_size);
					return true;
				}
				return false;
			}

			//获取共享指针对象。
			HX_INLINE::std::shared_ptr<T> share() HX_NOEXCEPT
			{
				m_refCnt.addRef();
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(m_data, [this](T* p) { release(); }, alloc);
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
					return ::std::shared_ptr<T>(m_data, [this](T* p) { release(); }, alloc);
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
					SharedBufferPoolAllocator<T2, MemoryFixedSizePoolResource<ObjMemGlobal>> alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T2>((T2*)m_data, [this](T2* p) { release(); }, alloc);
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
					return ::std::shared_ptr<T2>((T2*)m_data, [this](T2* p) { release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					m_refCnt.release();
					return ::std::shared_ptr<T2>();
				}
			}
		};

	private:
		//缓存池
		::std::shared_ptr<Pool<Buffer>> m_pool;

	public:
		//初始化缓存池对象。
		HX_INLINE FixedSizeBufferPool2(size_t size = 1, int limit = HX_FIXEDSIZE_POOL_LIMIT_NONE, int cache = 20) HX_NOEXCEPT
			: m_pool(Memory::makeSharedObject<Pool<Buffer>>(size, limit, cache < 0 ? 0 : cache))
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		HX_INLINE ~FixedSizeBufferPool2() HX_NOEXCEPT
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE FixedSizeBufferPool2(FixedSizeBufferPool2&& other) HX_NOEXCEPT
			: m_pool(::std::move(other.m_pool))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE FixedSizeBufferPool2& operator=(FixedSizeBufferPool2&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = ::std::move(other.m_pool);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE FixedSizeBufferPool2(const FixedSizeBufferPool2& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE FixedSizeBufferPool2& operator=(const FixedSizeBufferPool2& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool = other.m_pool;
			}
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(FixedSizeBufferPool2& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_pool.swap(other.m_pool);
			}
		}

		//获取缓存大小。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_pool->size();
		}
		//获取缓存字节大小。
		HX_INLINE size_t bytes() const HX_NOEXCEPT
		{
			return m_pool->bytes();
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

		//重置缓存。
		HX_INLINE void reset(size_t size, int limit = HX_FIXEDSIZE_POOL_LIMIT_RESERVED, int cache = HX_FIXEDSIZE_POOL_CACHE_RESERVED) HX_NOEXCEPT
		{
			m_pool->reset(size, limit, cache);
		}

		//获取一个缓存对象。
		HX_INLINE bool alloc(BufView<T>* bufView, Referencable** bufItem, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, reused);
			if (buf != nullptr)
			{
				auto buffer = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				*bufItem = buffer;
				bufView->reset(buffer->ptr(), buffer->size());
				return true;
			}
			*bufItem = nullptr;
			return false;
		}
		//获取一个缓存对象。
		HX_INLINE bool alloc(BufView<T>& bufView, Referencable** bufItem, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, reused);
			if (buf != nullptr)
			{
				auto buffer = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				*bufItem = buffer;
				bufView.reset(buffer->ptr(), buffer->size());
				return true;
			}
			*bufItem = nullptr;
			return false;
		}

		//获取一个缓存对象。
		HX_INLINE Buffer* alloc() HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
			}
			return nullptr;
		}
		//获取一个缓存对象。
		HX_INLINE Buffer* alloc(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				return new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
			}
			return nullptr;
		}

		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool* reused = nullptr) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, reused);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(::std::function<void()>&& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func)));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func));
				}
			}
			return value;
		}
		//获取一个缓存对象。
		HX_INLINE ObjRef<Buffer> objRef(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			ObjRef<Buffer> value;
			{
				size_t size, bytes; unsigned char* objBuf;
				unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
				if (buf != nullptr)
				{
					value.attach(new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func)));
				}
			}
			return value;
		}

		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		HX_INLINE::std::shared_ptr<T> share(bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
				try
				{
					SharedMemAlloc alloc(m_pool->sharedMemPool());
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}

		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool* reused = nullptr) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, nullptr);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused, const ::std::function<void()>& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, func);
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取一个缓存对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc, bool& reused, ::std::function<void()>&& func) HX_NOEXCEPT
		{
			size_t size, bytes; unsigned char* objBuf;
			unsigned char* buf = m_pool->alloc(size, bytes, &objBuf, &reused);
			if (buf != nullptr)
			{
				auto obj = new(objBuf)Buffer((T*)buf, size, bytes, m_pool, ::std::move(func));
				try
				{
					return ::std::shared_ptr<T>(obj->data(), [obj](T* p) { obj->release(); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					obj->release();
				}
			}
			return ::std::shared_ptr<T>();
		}

		//查询当前信息。
		HX_INLINE void queryInfo(size_t* size, int* limit, int* cache) const HX_NOEXCEPT
		{
			m_pool->query(size, limit, cache);
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
				m_pool->query(*state);
			}
		}
		//查询当前状态。
		HX_INLINE void queryState(State& state) const HX_NOEXCEPT
		{
			m_pool->query(state);
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
				m_pool->setSize(value);
			}
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
}

template <typename T, typename MemGlobal = HxMemoryGlobalDefault, size_t MemAlignment = HxMemoryAlignment<T>::value> 
using HxFixedSizeBufferPool = ::HxLibs::FixedSizeBufferPool<T, MemGlobal, MemAlignment>;

template <typename T, typename BufMemGlobal = HxMemoryGlobalDefault, typename ObjMemGlobal = HxMemoryGlobalDefault, size_t BufMemAlignment = HxMemoryAlignment<T>::value>
using HxFixedSizeBufferPool2 = ::HxLibs::FixedSizeBufferPool2<T, BufMemGlobal, ObjMemGlobal, BufMemAlignment>;

#endif //!HX_FIXEDSIZE_BUFFER_POOL
#endif //__cplusplus

#endif // !HX_CORE_FIXEDSIZE_BUFFER_POOL_H
