/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SIMPLE_BUFFER_H
#define HX_CORE_SIMPLE_BUFFER_H

#ifdef __cplusplus
#ifndef HX_SIMPLE_BUFFER
#define HX_SIMPLE_BUFFER

#include <stdint.h>
#include <type_traits>
#include "BufView.h"
#include "Uncopyable.h"
#include "MemoryAlignment.h"
#include "MemoryGlobalDefault.h"
#include "MemoryPackedAllocation.h"

namespace HxLibs
{
	//表示简单缓存。
	template <typename T, typename MemGlobal = MemoryGlobalDefault, size_t MemAlignment = MemoryAlignment<T>::value>
	class SimpleBuffer : Uncopyable
	{
		HX_STATIC_ASSERT((MemAlignment >= MemoryAlignment<T>::value), "::HxLibs::SimpleBuffer<> MemAlignment parameter must >= MemoryAlignment<T>::value.");
		HX_STATIC_ASSERT((checkPowerOf2(MemAlignment)), "::HxLibs::SimpleBuffer<> MemAlignment parameter must be a power of 2.");

	private:
		T* m_ptr;
		size_t m_size;

	public:
		//获取首地址。
		HX_INLINE T* ptr() HX_NOEXCEPT
		{
			return m_ptr;
		}
		//获取首地址。
		HX_INLINE const T* ptr() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//获取缓存容量。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_size;
		}

		//初始化空缓存。
		HX_INLINE SimpleBuffer() HX_NOEXCEPT
			: m_ptr(nullptr)
			, m_size(0)
		{
			//NOTHING
		}
		//初始化指定容量的缓存。
		HX_INLINE explicit SimpleBuffer(size_t size) HX_NOEXCEPT
		{
			if (size > 0)
			{
				m_ptr = (T*)(MemGlobal::allocate(size * sizeof(T), MemAlignment));
				m_size = m_ptr != nullptr ? size : 0;
			}
			else
			{
				m_ptr = nullptr;
				m_size = 0;
			}
		}

		//释放当前对象所占用的相关资源。
		HX_INLINE ~SimpleBuffer() HX_NOEXCEPT
		{
			if (m_size > 0)
			{
				MemGlobal::deallocate(m_ptr);
				m_ptr = nullptr;
				m_size = 0;
			}
		}

		//移动构造函数。
		HX_INLINE SimpleBuffer(SimpleBuffer&& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr)
			, m_size(other.m_size)
		{
			other.m_ptr = nullptr;
			other.m_size = 0;
		}
		//移动操作符。
		HX_INLINE SimpleBuffer& operator=(SimpleBuffer&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
				}
				m_ptr = other.m_ptr;
				m_size = other.m_size;
				other.m_ptr = nullptr;
				other.m_size = 0;
			}
			return *this;
		}

		//与指定缓存交换。
		HX_INLINE void swap(SimpleBuffer& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_ptr, other.m_ptr);
				::std::swap(m_size, other.m_size);
			}
		}

		//重置缓存为空。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			if (m_size > 0)
			{
				MemGlobal::deallocate(m_ptr);
				m_ptr = nullptr;
				m_size = 0;
			}
		}
		//重置缓存为指定容量。
		HX_INLINE void reset(size_t size) HX_NOEXCEPT
		{
			if (size != m_size)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
					m_ptr = nullptr;
					m_size = 0;
				}
				if (size > 0)
				{
					m_ptr = (T*)(MemGlobal::allocate(size * sizeof(T), MemAlignment));
					m_size = m_ptr != nullptr ? size : 0;
				}
			}
		}

		//重新分配，只有在原始容量不够或原始分配太大的情况下，才重新进行分配
		HX_INLINE void realloc(size_t size) HX_NOEXCEPT
		{
			if (size > m_size || size < m_size / 2)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
					m_ptr = nullptr;
					m_size = 0;
				}
				if (size > 0)
				{
					m_ptr = (T*)(MemGlobal::allocate(size * sizeof(T), MemAlignment));
					m_size = m_ptr != nullptr ? size : 0;
				}
			}
		}

		HX_INLINE operator BufView<T>() HX_NOEXCEPT
		{
			return BufView<T>(m_ptr, m_size);
		}
		HX_INLINE operator BufConstView<T>() const HX_NOEXCEPT
		{
			return BufConstView<T>(m_ptr, m_size);
		}

		HX_INLINE operator T* () HX_NOEXCEPT
		{
			return m_ptr;
		}
		HX_INLINE operator const T* () const HX_NOEXCEPT
		{
			return m_ptr;
		}

		HX_INLINE T* operator() () HX_NOEXCEPT
		{
			return m_ptr;
		}
		HX_INLINE const T* operator() () const HX_NOEXCEPT
		{
			return m_ptr;
		}

		template <typename TTo>
		HX_INLINE explicit operator TTo* () HX_NOEXCEPT
		{
			return (TTo*)m_ptr;
		}
		template <typename TTo>
		HX_INLINE explicit operator const TTo* () const HX_NOEXCEPT
		{
			return (const TTo*)m_ptr;
		}

		HX_INLINE bool operator==(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_ptr == nullptr;
		}
		HX_INLINE bool operator!=(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}

		HX_INLINE T& at(size_t idx) HX_NOEXCEPT
		{
			return m_ptr[idx];
		}
		HX_INLINE const T& at(size_t idx) const HX_NOEXCEPT
		{
			return m_ptr[idx];
		}
	};

	//表示简单缓存(void的特化版本)。
	template <typename MemGlobal, size_t MemAlignment>
	class SimpleBuffer<void, MemGlobal, MemAlignment> : Uncopyable
	{
		HX_STATIC_ASSERT((MemAlignment >= MemoryAlignment<void>::value), "::HxLibs::SimpleBuffer<void> MemAlignment parameter must >= MemoryAlignment<void>::value.");
		HX_STATIC_ASSERT((checkPowerOf2(MemAlignment)), "::HxLibs::SimpleBuffer<void> MemAlignment parameter must be a power of 2.");

	private:
		void* m_ptr;
		size_t m_size;

	public:
		//获取首地址。
		HX_INLINE void* ptr() HX_NOEXCEPT
		{
			return m_ptr;
		}
		//获取首地址。
		HX_INLINE const void* ptr() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//获取缓存容量。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_size;
		}

		//初始化空缓存。
		HX_INLINE SimpleBuffer() HX_NOEXCEPT
			: m_ptr(nullptr)
			, m_size(0)
		{
			//NOTHING
		}
		//初始化指定容量的缓存。
		HX_INLINE explicit SimpleBuffer(size_t size) HX_NOEXCEPT
		{
			if (size > 0)
			{
				m_ptr = MemGlobal::allocate(size, MemAlignment);
				m_size = m_ptr != nullptr ? size : 0;
			}
			else
			{
				m_ptr = nullptr;
				m_size = 0;
			}
		}
		//移动构造函数。
		HX_INLINE SimpleBuffer(SimpleBuffer&& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr)
			, m_size(other.m_size)
		{
			other.m_ptr = nullptr;
			other.m_size = 0;
		}

		//释放当前对象所占用的相关资源。
		HX_INLINE ~SimpleBuffer() HX_NOEXCEPT
		{
			if (m_size > 0)
			{
				MemGlobal::deallocate(m_ptr);
				m_ptr = nullptr;
				m_size = 0;
			}
		}

		//移动操作符。
		HX_INLINE SimpleBuffer& operator=(SimpleBuffer&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
				}
				m_ptr = other.m_ptr;
				m_size = other.m_size;
				other.m_ptr = nullptr;
				other.m_size = 0;
			}
			return *this;
		}

		//与指定缓存进行交换。
		HX_INLINE void swap(SimpleBuffer& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_ptr, other.m_ptr);
				::std::swap(m_size, other.m_size);
			}
		}

		//重置缓存为空。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			if (m_size > 0)
			{
				MemGlobal::deallocate(m_ptr);
				m_ptr = nullptr;
				m_size = 0;
			}
		}
		//重置缓存为指定容量。
		HX_INLINE void reset(size_t size) HX_NOEXCEPT
		{
			if (size != m_size)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
					m_ptr = nullptr;
					m_size = 0;
				}
				if (size > 0)
				{
					m_ptr = MemGlobal::allocate(size, MemAlignment);
					m_size = m_ptr != nullptr ? size : 0;
				}
			}
		}

		//重新分配，只有在原始容量不够或原始分配太大的情况下，才重新进行分配
		HX_INLINE void realloc(size_t size) HX_NOEXCEPT
		{
			if (size > m_size || size < m_size / 2)
			{
				if (m_size > 0)
				{
					MemGlobal::deallocate(m_ptr);
					m_ptr = nullptr;
					m_size = 0;
				}
				if (size > 0)
				{
					m_ptr = MemGlobal::allocate(size, MemAlignment);
					m_size = m_ptr != nullptr ? size : 0;
				}
			}
		}

		HX_INLINE void* operator+(size_t offset) HX_NOEXCEPT
		{
			return (char*)m_ptr + offset;
		}
		HX_INLINE const void* operator+(size_t offset) const HX_NOEXCEPT
		{
			return (char*)m_ptr + offset;
		}

		HX_INLINE operator BufView<void>() HX_NOEXCEPT
		{
			return BufView<void>(m_ptr, m_size);
		}
		HX_INLINE operator BufConstView<void>() const HX_NOEXCEPT
		{
			return BufConstView<void>(m_ptr, m_size);
		}

		HX_INLINE operator void* () HX_NOEXCEPT
		{
			return m_ptr;
		}
		HX_INLINE operator const void* () const HX_NOEXCEPT
		{
			return m_ptr;
		}

		HX_INLINE void* operator() () HX_NOEXCEPT
		{
			return m_ptr;
		}
		HX_INLINE const void* operator() () const HX_NOEXCEPT
		{
			return m_ptr;
		}

		template <typename TTo>
		HX_INLINE explicit operator TTo* () HX_NOEXCEPT
		{
			return (TTo*)m_ptr;
		}
		template <typename TTo>
		HX_INLINE explicit operator const TTo* () const HX_NOEXCEPT
		{
			return (const TTo*)m_ptr;
		}

		HX_INLINE bool operator==(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_ptr == nullptr;
		}
		HX_INLINE bool operator!=(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}

		//准备以打包形式分配内存。
		template <size_t MaxAlignment = MemAlignment>
		HX_INLINE MemoryPackedAllocation<MemGlobal, MaxAlignment> packing() HX_NOEXCEPT
		{
			HX_STATIC_ASSERT((MaxAlignment >= MemAlignment), "::HxLibs::SimpleBuffer<void>::packing() MaxAlignment parameter must >= MemAlignment.");
			HX_STATIC_ASSERT((checkPowerOf2(MaxAlignment)), "::HxLibs::SimpleBuffer<void>::packing() MaxAlignment parameter must be a power of 2.");

			return MemoryPackedAllocation<MemGlobal, MaxAlignment>((uintptr_t&)m_ptr, m_size);
		}
	};
}
template <typename T = void, typename MemGlobal = HxMemoryGlobalDefault, size_t MemAlignment = HxMemoryAlignment<T>::value>
using HxSimpleBuffer = ::HxLibs::SimpleBuffer<T, MemGlobal, MemAlignment>;

#endif //!HX_SIMPLE_BUFFER
#endif //__cplusplus

#endif //!HX_CORE_SIMPLE_BUFFER_H
