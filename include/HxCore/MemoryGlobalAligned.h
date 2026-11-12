/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_GLOBAL_ALIGNED_H
#define HX_CORE_MEMORY_GLOBAL_ALIGNED_H

#include "API.h"
#include "MemoryAPI.h"
#include "MemoryResource.h"

#ifndef HX_MEMORY_GLOBAL_ALIGNED
#define HX_MEMORY_GLOBAL_ALIGNED

 //{817C9181-F63D-4C78-B11C-FE8881A0ED9D}
#define HX_MEMORY_GLOBAL_RESOURCE_ALIGNED_FUNC 0x817c9181, 0xf63d, 0x4c78, 0xb1, 0x1c, 0xfe, 0x88, 0x81, 0xa0, 0xed, 0x9d
typedef HxMemoryResource* (HX_STDCALL* HxMemoryGlobalResourceAlignedFunc)();

#ifdef __cplusplus
namespace HxLibs
{
	//表示全局内存资源。
	struct MemoryGlobalAligned
	{
	private:
		//获取资源对象
		HX_INLINE static MemoryResource* getResource() HX_NOEXCEPT
		{
			auto func = queryFunction<HxMemoryGlobalResourceAlignedFunc>(Uuid(HX_MEMORY_GLOBAL_RESOURCE_ALIGNED_FUNC));
			return func();
		}

	public:
		//获取内存资源
		static MemoryResource* resource() HX_NOEXCEPT
		{
			static MemoryResource* _val = getResource();
			return _val;
		}

		//分配指定大小的内存
		HX_INLINE static void* allocate(size_t bytes, size_t align = HX_ALIGNOF(void*)) HX_NOEXCEPT
		{
			return ::HxCore_malloc_align(bytes, align);
		}
		//释放之前分配的内存
		HX_INLINE static void deallocate(void* ptr) HX_NOEXCEPT
		{
			if (ptr != nullptr)
			{
				::HxCore_mfree_align(ptr);
			}
		}
	};
}
using HxMemoryGlobalAligned = ::HxLibs::MemoryGlobalAligned;
#endif //__cplusplus

#endif //!HX_MEMORY_GLOBAL_ALIGNED

#endif //!HX_CORE_MEMORY_GLOBAL_ALIGNED_H
