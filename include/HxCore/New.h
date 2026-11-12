/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_NEW_H
#define HX_NEW_H

#ifdef __cplusplus

#ifdef HX_NEWDELETE_TRACE
#include <cstdio>
#endif //HX_NEWDELETE_TRACE
#include <exception>
#include "MemoryAPI.h"

#ifndef HX_NEWDELETE_TRACE
#ifdef HX_NEWDELETE_TESTS
#include <cstdio>
#define HX_NEWDELETE_TRACE
#endif //HX_NEWDELETE_TESTS
#endif // !HX_NEWDELETE_TRACE

void* operator new(size_t size)
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator new\n");
#endif //HX_NEWDELETE_TRACE
	auto buf = HxCore_malloc(size);
	if (buf == nullptr)
	{
		throw ::std::bad_alloc();
	}
	return buf;
}

void* operator new[](size_t size)
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator new[]\n");
#endif //HX_NEWDELETE_TRACE
	auto buf = HxCore_malloc(size);
	if (buf == nullptr)
	{
		throw ::std::bad_alloc();
	}
	return buf;
}

void* operator new(size_t size, ::std::nothrow_t) noexcept
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator new nothrow\n");
#endif //HX_NEWDELETE_TRACE
	return HxCore_malloc(size);
}

void* operator new[](size_t size, ::std::nothrow_t) noexcept
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator new[] nothrow\n");
#endif //HX_NEWDELETE_TRACE
	return HxCore_malloc(size);
}

void operator delete(void* ptr) noexcept
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator delete\n");
#endif //HX_NEWDELETE_TRACE
	HxCore_mfree(ptr);
}

void operator delete[](void* ptr) noexcept
{
#ifdef HX_NEWDELETE_TRACE
	printf("call operator delete[]\n");
#endif //HX_NEWDELETE_TRACE
	HxCore_mfree(ptr);
}

#endif //__cplusplus

#endif //HX_NEW_H
