/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_H
#define HX_CORE_MEMORY_H

#ifdef __cplusplus
#ifndef HX_MEMORY
#define HX_MEMORY

#include <cstdio>
#include <memory>
#include <cstring>
#include "MemoryGlobalDefault.h"
#include "MemoryGlobalAligned.h"
#include "MemoryGlobalAllocator.h"

#if HX_WINDOWS
#include <Windows.h>
#else //!HX_WINDOWS
#include <unistd.h>
#endif //HX_WINDOWS

namespace HxLibs
{
	//提供内存操作的相应方法。
	class Memory
	{
	private:
		//获取内存页大小。
		HX_INLINE static size_t getPageSize() HX_NOEXCEPT
		{
#if HX_WINDOWS
			SYSTEM_INFO sysInfo;
			GetSystemInfo(&sysInfo);
			return sysInfo.dwPageSize;
#else //!HX_WINDOWS
			return sysconf(_SC_PAGE_SIZE);
#endif // HX_WINDOWS
		}

	public:
		//分配指定大小的内存。
		HX_INLINE static void* alloc(size_t bytes) HX_NOEXCEPT
		{
			return MemoryGlobalDefault::allocate(bytes, 0);
		}
		//释放指定的内存对象。
		HX_INLINE static void free(void* p) HX_NOEXCEPT
		{
			MemoryGlobalDefault::deallocate(p);
		}

		//分配指定大小的对齐内存。
		HX_INLINE static void* allocAligned(size_t bytes, size_t align) HX_NOEXCEPT
		{
			return MemoryGlobalAligned::allocate(bytes, align);
		}
		//释放指定的对齐内存对象。
		HX_INLINE static void freeAligned(void* p) HX_NOEXCEPT
		{
			MemoryGlobalAligned::deallocate(p);
		}

		//获取内存页大小。
		static size_t pageSize() HX_NOEXCEPT
		{
			static size_t _val = getPageSize();
			return _val;
		}

		//释放指定的简单类型数组。
		template <typename T>
		HX_INLINE static void freeArray(T* p) HX_NOEXCEPT
		{
			if (p != nullptr)
			{
				Memory::free(p);
			}
		}
		//创建指定大小的简单类型数组。
		template <typename T>
		HX_INLINE static T* makeArray(size_t size) HX_NOEXCEPT
		{
			if (size > 0)
			{
				return (T*)Memory::alloc(sizeof(T) * size);
			}
			return nullptr;
		}

		//释放指定对象。
		template <typename T>
		HX_INLINE static void freeObject(T* obj) HX_NOEXCEPT
		{
			if (obj != nullptr)
			{
				obj->~T();
				Memory::free(obj);
			}
		}
		//创建指定对象。
		template <class T, class... TArgs>
		HX_INLINE static T* makeObject(TArgs&&... args) HX_NOEXCEPT
		{
			void* buffer = Memory::alloc(sizeof(T));
			if (buffer != nullptr)
			{
				try
				{
					return new(buffer)T(::std::forward<TArgs>(args)...);
				}
				catch (...)
				{
					Memory::free(buffer);
				}
			}
			return nullptr;
		}

		//释放指定的对象数组。
		template <typename T>
		HX_INLINE static void freeObjectArray(T* p) HX_NOEXCEPT
		{
			if (p != nullptr)
			{
				void* buffer = (unsigned char*)p - sizeof(size_t);
				size_t size = *(size_t*)buffer;
				for (size_t i = 0; i < size; ++i)
				{
					p[i].~T();
				}
				Memory::free(buffer);
			}
		}
		//创建指定大小的对象数组。
		template <typename T>
		HX_INLINE static T* makeObjectArray(size_t size) HX_NOEXCEPT
		{
			if (size > 0)
			{
				void* buffer = Memory::alloc(sizeof(T) * size + sizeof(size_t));
				if (buffer != nullptr)
				{
					T* data = (T*)((unsigned char*)buffer + sizeof(size_t));
					for (size_t i = 0; i < size; ++i)
					{
						try
						{
							new(&data[i])T();
						}
						catch (...)
						{
							for (size_t j = 0; j < i; ++j)
							{
								data[j].~T();
							}
							Memory::free(buffer);
							return nullptr;
						}
					}
					*(size_t*)buffer = size;
					return data;
				}
			}
			return nullptr;
		}

		//创建共享简单数组。
		template <typename T>
		HX_INLINE static ::std::shared_ptr<T> makeSharedArray(size_t size) HX_NOEXCEPT
		{
			T* arr = makeArray<T>(size);
			if (arr != nullptr)
			{
				try
				{
					MemoryGlobalAllocator<T> alloc;
					return ::std::shared_ptr<T>(arr, [](T* p) { freeArray(p); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					freeArray(arr);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//创建共享简单数组。
		template <typename T, typename Alloc>
		HX_INLINE static ::std::shared_ptr<T> makeSharedArray(const Alloc& alloc, size_t size) HX_NOEXCEPT
		{
			T* arr = makeArray<T>(size);
			if (arr != nullptr)
			{
				try
				{
					return ::std::shared_ptr<T>(arr, [](T* p) { freeArray(p); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					freeArray(arr);
				}
			}
			return ::std::shared_ptr<T>();
		}

		//创建共享对象。
		template<class T, class... TArgs>
		HX_INLINE static ::std::shared_ptr<T> makeSharedObject(TArgs&&... args) HX_NOEXCEPT
		{
			try
			{
				MemoryGlobalAllocator<T> alloc;
				return ::std::allocate_shared<T, MemoryGlobalAllocator<T>, TArgs...>(alloc, ::std::forward<TArgs>(args)...);
			}
			catch (...)//::std::bad_alloc
			{
				return ::std::shared_ptr<T>();
			}
		}

		//创建共享对象数组。
		template <typename T>
		HX_INLINE static ::std::shared_ptr<T> makeSharedObjectArray(size_t size) HX_NOEXCEPT
		{
			T* arr = makeObjectArray<T>(size);
			if (arr != nullptr)
			{
				try
				{
					MemoryGlobalAllocator<T> alloc;
					return ::std::shared_ptr<T>(arr, [](T* p) { freeObjectArray(p); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					freeObjectArray(arr);
				}
			}
			return ::std::shared_ptr<T>();
		}
		//创建共享对象数组。
		template <typename T, typename Alloc>
		HX_INLINE static ::std::shared_ptr<T> makeSharedObjectArray(const Alloc& alloc, size_t size) HX_NOEXCEPT
		{
			T* arr = makeObjectArray<T>(size);
			if (arr != nullptr)
			{
				try
				{
					return ::std::shared_ptr<T>(arr, [](T* p) { freeObjectArray(p); }, alloc);
				}
				catch (...)//::std::bad_alloc
				{
					freeObjectArray(arr);
				}
			}
			return ::std::shared_ptr<T>();
		}
	};
}
using HxMemory = ::HxLibs::Memory;

#endif //!HX_MEMORY
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_H
