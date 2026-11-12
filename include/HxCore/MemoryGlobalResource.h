/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_GLOBAL_RESOURCE_H
#define HX_CORE_MEMORY_GLOBAL_RESOURCE_H

#ifdef __cplusplus
#ifndef HX_MEMORY_GLOBAL_RESOURCE
#define HX_MEMORY_GLOBAL_RESOURCE

#include "Uncopyable.h"
#include "MemoryResource.h"
#include "MemoryGlobalDefault.h"
#if _HAS_CXX17
#include <memory_resource>
#endif //_HAS_CXX17

namespace HxLibs
{
	//表示全局内存资源。
	template <typename Global = MemoryGlobalDefault>
	struct MemoryGlobalResource : Uncopyable, public MemoryResource
#if _HAS_CXX17
		, public ::std::pmr::memory_resource
#endif //_HAS_CXX17
	{
	public:
		//初始化当前对象。
		HX_INLINE MemoryGlobalResource() HX_NOEXCEPT
		{
			//NOTHING
		}

		//分配指定大小的内存
		virtual void* HX_STDCALL allocate(size_t bytes, size_t align) HX_NOEXCEPT override
		{
			return Global::allocate(bytes, align);
		}
		//释放之前分配的内存
		virtual void HX_STDCALL deallocate(void* ptr, size_t bytes, size_t align) HX_NOEXCEPT override
		{
			Global::deallocate(ptr);
		}

#if _HAS_CXX17
	private:
		virtual void* do_allocate(size_t bytes, size_t align) override
		{
			return Global::allocate(bytes, align);
		}
		virtual void do_deallocate(void* ptr, size_t bytes, size_t align) override
		{
			Global::deallocate(ptr);
		}
		virtual bool do_is_equal(const ::std::pmr::memory_resource& other) const noexcept override
		{
			return &other == (::std::pmr::memory_resource*)this;
		}
#endif //_HAS_CXX17
	};
}
template <typename Global = HxMemoryGlobalDefault>
using HxMemoryGlobalResource = ::HxLibs::MemoryGlobalResource<Global>;

#endif //HX_MEMORY_GLOBAL_RESOURCE
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_GLOBAL_RESOURCE_H
