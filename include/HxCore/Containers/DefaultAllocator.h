/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_DEFAULT_ALLOCATOR_H
#define HX_CORE_CONTAINERS_DEFAULT_ALLOCATOR_H

#ifdef __cplusplus
#ifndef HX_CONTAINERS_DEFAULT_ALLOCATOR
#define HX_CONTAINERS_DEFAULT_ALLOCATOR

#ifdef HX_CONTAINERS_CUSTOM
#include "../MemoryResourceAllocator.h"
namespace HxLibs
{
	namespace Containers
	{
		template <typename T>
		using DefaultAllocator = MemoryResourceAllocator<T>;
	}
}
#else //!HX_CONTAINERS_CUSTOM
#	if _HAS_CXX17 && defined HX_CONTAINERS_STDPMR
#include <memory_resource>
namespace HxLibs
{
	namespace Containers
	{
		template <typename T>
		using DefaultAllocator = ::std::pmr::polymorphic_allocator<T>;
	}
}
#	else //!HX_CONTAINERS_STDPMR
#include <memory>
namespace HxLibs
{
	namespace Containers
	{
		template <typename T>
		using DefaultAllocator = ::std::allocator<T>;
	}
}
#	endif //!HX_CONTAINERS_STDPMR
#endif //!HX_CONTAINERS_CUSTOM

namespace HxLibs
{
	template <typename T>
	using ContainerDefaultAllocator = ::HxLibs::Containers::DefaultAllocator<T>;
}
template <typename T>
using HxContainerDefaultAllocator = ::HxLibs::Containers::DefaultAllocator<T>;

#endif //!HX_CONTAINERS_DEFAULT_ALLOCATOR
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_DEFAULT_ALLOCATOR_H
