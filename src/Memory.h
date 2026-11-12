/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_SRC_H
#define HX_CORE_MEMORY_SRC_H

#include <HxCore/MemoryAPI.h>
#include <HxCore/MemoryGlobalResource.h>
#include <HxCore/MemoryAllocPerfMonitoring.h>

namespace HxLibs
{
	namespace Current
	{
		MemoryResource* HX_STDCALL globalMemoryResourceDefault() HX_NOEXCEPT;
		MemoryResource* HX_STDCALL globalMemoryResourceAligned() HX_NOEXCEPT;

		MemoryAllocPerfMonitoring* HX_STDCALL globalMemoryAllocPerfMonitoring() HX_NOEXCEPT;
	}
}

#endif //!HX_CORE_MEMORY_SRC_H
