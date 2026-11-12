/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_UNCOPYABLE_H
#define HX_CORE_UNCOPYABLE_H

#ifdef __cplusplus
#ifndef HX_UNCOPYABLE
#define HX_UNCOPYABLE

#include "Defines.h"

namespace HxLibs
{
	struct Uncopyable
	{//禁用复制构造函数和复制赋值操作符。
	private:		
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;

	protected:
		HX_INLINE Uncopyable() HX_NOEXCEPT
		{
			//NOTHING
		}
	};
}
using HxUncopyable = ::HxLibs::Uncopyable;

#endif //!HX_UNCOPYABLE
#endif //__cplusplus

#endif //!HX_CORE_UNCOPYABLE_H
