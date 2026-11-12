/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRING_POOL_SRC_H
#define HX_CORE_STRING_POOL_SRC_H

#include <HxCore/StringPool.h>

namespace HxLibs
{
	namespace Current
	{
		StringPool* HX_STDCALL defaultStringPool() HX_NOEXCEPT;
		WStringPool* HX_STDCALL defaultWStringPool() HX_NOEXCEPT;
	}
}

#endif //!HX_CORE_STRING_POOL_SRC_H
