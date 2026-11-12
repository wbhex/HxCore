/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_GLOBAL_DEFAULT_H
#define HX_CORE_MEMORY_GLOBAL_DEFAULT_H

#include "API.h"
#include "MemoryAPI.h"
#include "MemoryResource.h"

#ifndef HX_MEMORY_GLOBAL_DEFAULT
#define HX_MEMORY_GLOBAL_DEFAULT

 //{33E7B038-DE0D-4669-9D43-21E95806CD14}
#define HX_MEMORY_GLOBAL_RESOURCE_DEFAULT_FUNC 0x33e7b038, 0xde0d, 0x4669, 0x9d, 0x43, 0x21, 0xe9, 0x58, 0x6, 0xcd, 0x14
typedef HxMemoryResource* (HX_STDCALL* HxMemoryGlobalResourceDefaultFunc)();

#ifdef __cplusplus
namespace HxLibs
{
	//表示全局内存资源。
	struct MemoryGlobalDefault
	{
	private:
		//获取资源对象
		HX_INLINE static MemoryResource* getResource() HX_NOEXCEPT
		{
			auto func = queryFunction<HxMemoryGlobalResourceDefaultFunc>(Uuid(HX_MEMORY_GLOBAL_RESOURCE_DEFAULT_FUNC));
			return func();
		}

	public:
		//获取资源对象
		static MemoryResource* resource() HX_NOEXCEPT
		{
			static MemoryResource* _val = getResource();
			return _val;
		}

		//分配指定大小的内存
		HX_INLINE static void* allocate(size_t bytes, size_t = 0) HX_NOEXCEPT
		{
			return ::HxCore_malloc(bytes);
		}
		//释放之前分配的内存
		HX_INLINE static void deallocate(void* ptr) HX_NOEXCEPT
		{
			if (ptr != nullptr)
			{
				::HxCore_mfree(ptr);
			}
		}
	};
}
using HxMemoryGlobalDefault = ::HxLibs::MemoryGlobalDefault;
#endif //__cplusplus

#endif //!HX_MEMORY_GLOBAL_DEFAULT

#endif //!HX_CORE_MEMORY_GLOBAL_DEFAULT_H
