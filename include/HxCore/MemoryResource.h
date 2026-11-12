/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_RESOURCE_H
#define HX_CORE_MEMORY_RESOURCE_H

#ifndef HX_MEMORY_RESOURCE
#define HX_MEMORY_RESOURCE

#include "Referencable.h"

//表示内存资源接口。
HX_INTERFACE_DECLARE(HxMemoryResource)
{
	//分配指定大小的内存，如果分配失败则返回NULL。
	HX_METHOD(void*, allocate)(HX_THIS_(HxMemoryResource) size_t bytes, size_t align) HX_NOEXCEPT HX_PURE;
	//释放之前分配的内存
	HX_METHOD(void, deallocate)(HX_THIS_(HxMemoryResource) void* ptr, size_t bytes, size_t align) HX_NOEXCEPT HX_PURE;
};
//表示内存资源引用接口。
HX_INTERFACE_ALIAS(HxReferencable, HxMemoryResourceReferencable);

#ifdef __cplusplus
namespace HxLibs
{
	using MemoryResource = HxMemoryResource;
	using MemoryResourceReferencable = HxMemoryResourceReferencable;
}
#endif //__cplusplus

#endif //!HX_MEMORY_RESOURCE

#endif //!HX_CORE_MEMORY_RESOURCE_H
