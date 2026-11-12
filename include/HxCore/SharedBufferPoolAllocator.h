/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SHARED_BUFFER_POOL_ALLOCATOR_H
#define HX_CORE_SHARED_BUFFER_POOL_ALLOCATOR_H

#ifndef HX_SHARED_BUFFER_POOL_ALLOCATOR
#define HX_SHARED_BUFFER_POOL_ALLOCATOR

#ifdef __cplusplus
#include <memory>
#include <functional>
#include <type_traits>
#include "Defines.h"
#include "ByteLength.h"

namespace HxLibs
{
	template <typename T, typename Pool>
	class SharedBufferPoolAllocator0
	{
		template <typename, typename>
		friend class SharedBufferPoolAllocator0;

	private:
		::std::shared_ptr<Pool> m_pool;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = SharedBufferPoolAllocator0<T2, Pool>;
		};

		template <class TObj, class... TArgs>
		HX_INLINE void construct(TObj* const p, TArgs&&... args)
		{
			::new (p) TObj(::std::forward<TArgs>(args)...);
		}

		template <class TObj>
		HX_INLINE void destroy(TObj* const p) HX_NOEXCEPT
		{
			p->~TObj();
		}

		//初始化
		HX_INLINE explicit SharedBufferPoolAllocator0(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: m_pool(pool)
		{
			//NOTHING
		}
		//复制构造函数
		HX_INLINE SharedBufferPoolAllocator0(const SharedBufferPoolAllocator0& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING
		}
		//复制构造函数
		template <typename T2>
		HX_INLINE SharedBufferPoolAllocator0(const SharedBufferPoolAllocator0<T2, Pool>& other) HX_NOEXCEPT
			: m_pool(other.m_pool)
		{
			//NOTHING
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const SharedBufferPoolAllocator0<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool == other.m_pool;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const SharedBufferPoolAllocator0<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool != other.m_pool;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = m_pool->alloc(ByteLength<T>::count(n));
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
		{
			m_pool->free((unsigned char*)ptr, ByteLength<T>::count(n));
		}
	};


	template <typename T, typename Pool>
	class SharedBufferPoolAllocator1_1
	{
		template <typename, typename>
		friend class SharedBufferPoolAllocator1_1;

	private:
		::std::shared_ptr<Pool> m_pool;
		::std::function<void()> m_func;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = SharedBufferPoolAllocator1_1<T2, Pool>;
		};

		template <class TObj, class... TArgs>
		HX_INLINE void construct(TObj* const p, TArgs&&... args)
		{
			::new (p) TObj(::std::forward<TArgs>(args)...);
		}

		template <class TObj>
		HX_INLINE void destroy(TObj* const p) HX_NOEXCEPT
		{
			p->~TObj();
		}

		//初始化
		HX_INLINE explicit SharedBufferPoolAllocator1_1(const ::std::shared_ptr<Pool>& pool) HX_NOEXCEPT
			: m_pool(pool)
		{
			//NOTHING
		}
		//初始化
		HX_INLINE SharedBufferPoolAllocator1_1(const ::std::shared_ptr<Pool>& pool, ::std::function<void()>&& func) HX_NOEXCEPT
			: m_pool(pool), m_func(::std::move(func))
		{
			//NOTHING
		}
		HX_INLINE SharedBufferPoolAllocator1_1(const ::std::shared_ptr<Pool>& pool, const ::std::function<void()>& func) HX_NOEXCEPT
			: m_pool(pool), m_func(func)
		{
			//NOTHING
		}

		//复制构造函数
		HX_INLINE SharedBufferPoolAllocator1_1(const SharedBufferPoolAllocator1_1& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_func(other.m_func)
		{
			//NOTHING
		}
		//复制构造函数
		template <typename T1>
		HX_INLINE SharedBufferPoolAllocator1_1(const SharedBufferPoolAllocator1_1<T1, Pool>& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_func(other.m_func)
		{
			//NOTHING
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const SharedBufferPoolAllocator1_1<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool == other.m_pool && m_func == other.m_func;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const SharedBufferPoolAllocator1_1<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool != other.m_pool && m_func != other.m_func;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = m_pool->alloc(ByteLength<T>::count(n));
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
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
			m_pool->free((unsigned char*)ptr, ByteLength<T>::count(n));
		}
	};

	template <typename T, typename Pool>
	class SharedBufferPoolAllocator1_2
	{
		template <typename, typename>
		friend class SharedBufferPoolAllocator1_2;

	private:
		::std::shared_ptr<Pool> m_pool;
		bool* m_reused;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = SharedBufferPoolAllocator1_2<T2, Pool>;
		};

		template <class TObj, class... TArgs>
		HX_INLINE void construct(TObj* const p, TArgs&&... args)
		{
			::new (p) TObj(::std::forward<TArgs>(args)...);
		}

		template <class TObj>
		HX_INLINE void destroy(TObj* const p) HX_NOEXCEPT
		{
			p->~TObj();
		}

		//初始化
		HX_INLINE SharedBufferPoolAllocator1_2(const ::std::shared_ptr<Pool>& pool, bool* reused = nullptr) HX_NOEXCEPT
			: m_pool(pool), m_reused(reused)
		{
			//NOTHING
		}

		//复制构造函数
		HX_INLINE SharedBufferPoolAllocator1_2(const SharedBufferPoolAllocator1_2& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_reused(other.m_reused)
		{
			//NOTHING
		}
		//复制构造函数
		template <typename T1>
		HX_INLINE SharedBufferPoolAllocator1_2(const SharedBufferPoolAllocator1_2<T1, Pool>& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_reused(other.m_reused)
		{
			//NOTHING
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const SharedBufferPoolAllocator1_2<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool == other.m_pool && m_reused == other.m_reused;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const SharedBufferPoolAllocator1_2<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool != other.m_pool && m_reused != other.m_reused;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = m_pool->alloc(ByteLength<T>::count(n), m_reused);
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
		{
			m_pool->free((unsigned char*)ptr, ByteLength<T>::count(n));
		}
	};

	template <typename T, typename Pool>
	using SharedBufferPoolAllocator1 = SharedBufferPoolAllocator1_1<T, Pool>;


	template <typename T, typename Pool>
	class SharedBufferPoolAllocator2
	{
		template <typename, typename>
		friend class SharedBufferPoolAllocator2;

	private:
		::std::shared_ptr<Pool> m_pool;
		::std::function<void()> m_func;
		bool* m_reused;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = SharedBufferPoolAllocator2<T2, Pool>;
		};

		template <class TObj, class... TArgs>
		HX_INLINE void construct(TObj* const p, TArgs&&... args)
		{
			::new (p) TObj(::std::forward<TArgs>(args)...);
		}

		template <class TObj>
		HX_INLINE void destroy(TObj* const p) HX_NOEXCEPT
		{
			p->~TObj();
		}

		//初始化
		HX_INLINE SharedBufferPoolAllocator2(const ::std::shared_ptr<Pool>& pool, bool* reused = nullptr) HX_NOEXCEPT
			: m_pool(pool), m_reused(reused)
		{
			//NOTHING
		}
		//初始化
		HX_INLINE SharedBufferPoolAllocator2(const ::std::shared_ptr<Pool>& pool, ::std::function<void()>&& func, bool* reused = nullptr) HX_NOEXCEPT
			: m_pool(pool), m_func(::std::move(func)), m_reused(reused)
		{
			//NOTHING
		}
		HX_INLINE SharedBufferPoolAllocator2(const ::std::shared_ptr<Pool>& pool, const ::std::function<void()>& func, bool* reused = nullptr) HX_NOEXCEPT
			: m_pool(pool), m_func(func), m_reused(reused)
		{
			//NOTHING
		}

		//复制构造函数
		HX_INLINE SharedBufferPoolAllocator2(const SharedBufferPoolAllocator2& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_func(other.m_func), m_reused(other.m_reused)
		{
			//NOTHING
		}
		//复制构造函数
		template <typename T2>
		HX_INLINE SharedBufferPoolAllocator2(const SharedBufferPoolAllocator2<T2, Pool>& other) HX_NOEXCEPT
			: m_pool(other.m_pool), m_func(other.m_func), m_reused(other.m_reused)
		{
			//NOTHING
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const SharedBufferPoolAllocator2<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool == other.m_pool && m_func == other.m_func && m_reused == other.m_reused;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const SharedBufferPoolAllocator2<T2, Pool>& other) const HX_NOEXCEPT
		{
			return m_pool != other.m_pool && m_func != other.m_func && m_reused != other.m_reused;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = m_pool->alloc(ByteLength<T>::count(n), m_reused);
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
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
			m_pool->free((unsigned char*)ptr, ByteLength<T>::count(n));
		}
	};


	template <typename T, typename Pool> 
	using SharedBufferPoolAllocator = SharedBufferPoolAllocator0<T, Pool>;
}

template <typename T, typename Pool>
using HxSharedBufferPoolAllocator = ::HxLibs::SharedBufferPoolAllocator<T, Pool>;

template <typename T, typename Pool>
using HxSharedBufferPoolAllocator0 = ::HxLibs::SharedBufferPoolAllocator0<T, Pool>;

template <typename T, typename Pool>
using HxSharedBufferPoolAllocator1 = ::HxLibs::SharedBufferPoolAllocator1<T, Pool>;
template <typename T, typename Pool>
using HxSharedBufferPoolAllocator1_1 = ::HxLibs::SharedBufferPoolAllocator1_1<T, Pool>;
template <typename T, typename Pool>
using HxSharedBufferPoolAllocator1_2 = ::HxLibs::SharedBufferPoolAllocator1_2<T, Pool>;

template <typename T, typename Pool>
using HxSharedBufferPoolAllocator2 = ::HxLibs::SharedBufferPoolAllocator2<T, Pool>;

#endif //__cplusplus

#endif //!HX_SHARED_BUFFER_POOL_ALLOCATOR

#endif // !HX_CORE_SHARED_BUFFER_POOL_ALLOCATOR_H
