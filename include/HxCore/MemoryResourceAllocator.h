/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_RESOURCE_ALLOCATOR_H
#define HX_CORE_MEMORY_RESOURCE_ALLOCATOR_H

#ifdef __cplusplus
#ifndef HX_MEMORY_RESOURCE_ALLOCATOR
#define HX_MEMORY_RESOURCE_ALLOCATOR

#include <exception>
#include <type_traits>
#include "ByteLength.h"
#include "MemoryAlignment.h"
#include "MemoryGlobalDefault.h"

namespace HxLibs
{
	//表示内存资源分配类。
	template <typename T>
	class MemoryResourceAllocator
	{
		template <typename>
		friend class MemoryResourceAllocator;

	private:
		MemoryResource* m_resource;
		MemoryResourceReferencable* m_referencable;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = MemoryResourceAllocator<T2>;
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

		//释放当前对象所占用的相应资源
		HX_INLINE ~MemoryResourceAllocator()
		{
			m_resource = nullptr;
			if (m_referencable != nullptr)
			{
				m_referencable->release();
				m_referencable = nullptr;
			}
		}

		//初始化当前对象
		HX_INLINE MemoryResourceAllocator() HX_NOEXCEPT
			: m_resource(MemoryGlobalDefault::resource()), m_referencable(nullptr)
		{
			//NOTHING
		}
		//初始化当前对象
		HX_INLINE MemoryResourceAllocator(MemoryResource& resource, MemoryResourceReferencable* referencable = nullptr) HX_NOEXCEPT
			: m_resource(&resource), m_referencable(referencable)
		{
			//NOTHING
		}

		//复制构造函数
		template <typename T2>
		HX_INLINE MemoryResourceAllocator(const MemoryResourceAllocator<T2>& other) HX_NOEXCEPT
			: m_resource(other.m_resource), m_referencable(other.m_referencable)
		{
			if (m_referencable != nullptr)
			{
				m_referencable->addRef();
			}
		}
		//复制赋值操作符
		template <typename T2>
		HX_INLINE MemoryResourceAllocator& operator=(const MemoryResourceAllocator<T2>& other) HX_NOEXCEPT
		{
			if (m_resource != other.m_resource)
			{
				if (m_referencable != nullptr)
				{
					m_referencable->release();
				}
				m_resource = other.m_resource;
				m_referencable = other.m_referencable;
				if (m_referencable != nullptr)
				{
					m_referencable->addRef();
				}
			}
			return *this;
		}

		//执行交换处理。
		template <typename T2>
		HX_INLINE void swap(MemoryResourceAllocator<T2>& other) HX_NOEXCEPT
		{
			::std::swap(m_resource, other.m_resource);
			::std::swap(m_referencable, other.m_referencable);
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const MemoryResourceAllocator<T2>& other) const HX_NOEXCEPT
		{
			return m_resource == other.m_resource;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const MemoryResourceAllocator<T2>& other) const HX_NOEXCEPT
		{
			return m_resource != other.m_resource;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = m_resource->allocate(ByteLength<T>::count(n), MemoryAlignment<T>::value);
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
		{
			m_resource->deallocate(ptr, ByteLength<T>::count(n), MemoryAlignment<T>::value);
		}
	};
}
template <typename T>
using HxMemoryResourceAllocator = ::HxLibs::MemoryResourceAllocator<T>;

#endif //!HX_MEMORY_RESOURCE_ALLOCATOR
#endif //__cplusplus

#endif // !HX_CORE_MEMORY_RESOURCE_ALLOCATOR_H
