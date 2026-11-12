/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_REFCNT_H
#define HX_REFCNT_H

#ifdef __cplusplus
#include <atomic>
#include "Uncopyable.h"
namespace HxLibs
{
	//表示引用计数。
	struct RefCnt : Uncopyable
	{
	private:
#if HX_64BIT
		using Value = long long;
#else //!HX_64BIT
		using Value = int;
#endif //!HX_64BIT
		::std::atomic<Value> m_v;

	public:
		//构造函数。
		HX_INLINE RefCnt() HX_NOEXCEPT
			: m_v(1)
		{
			//NOTHING
		}

		//添加引用。
		HX_INLINE void addRef() HX_NOEXCEPT
		{
			++m_v;
		}
		//释放一次引用，如果计数变为0则返回TRUE。
		HX_INLINE bool release() HX_NOEXCEPT
		{
			return (m_v--) == 1;
		}
	};
}
using HxRefCnt = ::HxLibs::RefCnt;
#endif //__cplusplus

#endif //!HX_REFCNT_H
