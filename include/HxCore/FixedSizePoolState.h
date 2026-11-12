/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_FIXEDSIZE_POOL_STATE_H
#define HX_CORE_FIXEDSIZE_POOL_STATE_H

#ifndef HX_FIXEDSIZE_POOL_STATE
#define HX_FIXEDSIZE_POOL_STATE

#include "Struct.h"
#include "Defines.h"

 //表示固定大小池状态。
HX_LIBS_STRUCT_DECLARE(FixedSizePoolState)
{
	//缓存大小。
	size_t size;
	//缓存上限：负数表示不限制
	int limit;
	//可缓存数。
	int cache;
	//活动数量。
	int active;
	//缓存数量。
	int cached;
	//释放计数(负数表示未统计)
	long long free;
	//分配计数(负数表示未统计)
	long long alloc;
	//复用计数(负数表示未统计)
	long long reused;
#ifdef __cplusplus
	//初始化状态。
	HX_INLINE HX_LIBS_STRUCT_GLOBAL_NAME(FixedSizePoolState)() HX_NOEXCEPT
	{
		//NOTHING
	}
	//初始化状态。
	HX_CONSTEXPR11_FUNC HX_LIBS_STRUCT_GLOBAL_NAME(FixedSizePoolState)(size_t size, int limit, int cache, int active, int cached) HX_NOEXCEPT
		: size(size), limit(limit), cache(cache), active(active), cached(cached), free(-1), alloc(-1), reused(-1)
	{
		//NOTHING
	}

	//获取使用率（0.0 - 1.0）。
	HX_CONSTEXPR11_FUNC double usage() const HX_NOEXCEPT
	{ 
		return limit <= 0 ? 0.0 : active / (double)limit;
	}
#endif //__cplusplus
};

#endif //!HX_FIXEDSIZE_POOL_STATE

#endif //!HX_CORE_FIXEDSIZE_POOL_STATE_H
