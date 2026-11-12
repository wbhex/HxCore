/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_GLOBAL_ALLOCATOR_H
#define HX_CORE_MEMORY_GLOBAL_ALLOCATOR_H

#ifdef __cplusplus
#ifndef HX_MEMORY_GLOBAL_ALLOCATOR
#define HX_MEMORY_GLOBAL_ALLOCATOR

#include <exception>
#include "ByteLength.h"
#include "MemoryAlignment.h"
#include "MemoryGlobalDefault.h"

namespace HxLibs
{
	//表示全局内存分配类。
	template <typename T, typename Global = MemoryGlobalDefault>
	class MemoryGlobalAllocator
	{
		template <typename, typename>
		friend class MemoryGlobalAllocator;

	public:
		using value_type = T;

		template <class T2> struct rebind
		{
			using other = MemoryGlobalAllocator<T2, Global>;
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

		//初始化当前对象
		HX_INLINE MemoryGlobalAllocator() HX_NOEXCEPT
		{
			//NOTHING
		}

		//复制构造函数
		template <typename T2>
		HX_INLINE MemoryGlobalAllocator(const MemoryGlobalAllocator<T2>& other) HX_NOEXCEPT
		{
			//NOTHING
		}
		//复制赋值操作符
		template <typename T2>
		HX_INLINE MemoryGlobalAllocator& operator=(const MemoryGlobalAllocator<T2>& other) HX_NOEXCEPT
		{
			return *this;
		}

		//执行交换处理。
		template <typename T2>
		HX_INLINE void swap(MemoryGlobalAllocator<T2>& other) HX_NOEXCEPT
		{
			//NOTHING
		}

		//等于运算符
		template <typename T2>
		HX_INLINE bool operator==(const MemoryGlobalAllocator<T2>& other) const HX_NOEXCEPT
		{
			return true;
		}
		//不等于运算符
		template <typename T2>
		HX_INLINE bool operator!=(const MemoryGlobalAllocator<T2>& other) const HX_NOEXCEPT
		{
			return false;
		}

		//分配指定数量的缓存块
		HX_INLINE T* allocate(size_t n)
		{
			auto buf = Global::allocate(ByteLength<T>::count(n), MemoryAlignment<T>::value);
			if (buf == nullptr)
			{
				throw ::std::bad_alloc();
			}
			return (T*)(buf);
		}
		//释放指定数量的缓存块
		HX_INLINE void deallocate(T* ptr, size_t n) HX_NOEXCEPT
		{
			Global::deallocate(ptr);
		}
	};
}
template <typename T, typename Global = HxMemoryGlobalDefault>
using HxMemoryGlobalAllocator = ::HxLibs::MemoryGlobalAllocator<T, Global>;

#endif //!HX_MEMORY_GLOBAL_ALLOCATOR
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_GLOBAL_ALLOCATOR_H
