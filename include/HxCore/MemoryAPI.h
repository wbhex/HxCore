/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_API_H
#define HX_CORE_MEMORY_API_H

#include "Defines.h"

#ifndef HX_CORE_API
#define HX_CORE_API
#endif //!HX_CORE_API

 /*
 * 本头文件为内存分配管理核心头文件，其内存管理逻辑如下：
 *	如果当前库对应目录存在libjemalloc库，将优先使用该库进行内存管理
 *	如果当前库对应目录不存在libjemalloc库但存在libtcmalloc库，将使用该库进行内存管理
 *	如果以上两个库都不存在，将调用C运行时库进行内存管理
 */

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	//分配指定大小的内存。
	HX_CORE_API void* HX_STDCALL HxCore_malloc(size_t) HX_NOEXCEPT;
	//释放指定的内存对象。
	HX_CORE_API void HX_STDCALL HxCore_mfree(void*) HX_NOEXCEPT;

	//分配指定大小的对齐内存。
	HX_CORE_API void* HX_STDCALL HxCore_malloc_align(size_t, size_t align) HX_NOEXCEPT;
	//释放指定的对齐内存对象。
	HX_CORE_API void HX_STDCALL HxCore_mfree_align(void*) HX_NOEXCEPT;

#ifdef __cplusplus
} //extern "C"
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_API_H
