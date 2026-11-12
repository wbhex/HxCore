/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_FIXEDSIZE_POOL_FLAGS_H
#define HX_CORE_FIXEDSIZE_POOL_FLAGS_H

#ifndef HX_FIXEDSIZE_POOL_FLAGS
#define HX_FIXEDSIZE_POOL_FLAGS

#include "Defines.h"

#ifdef __cplusplus

//固定大小池缓存大小不重置
static HX_CONSTEXPR size_t HX_FIXEDSIZE_POOL_SIZE_RESERVED = 0;

//固定大小池容量上限不限制
static HX_CONSTEXPR int HX_FIXEDSIZE_POOL_LIMIT_NONE = -1;
//固定大小池容量上限不重置
static HX_CONSTEXPR int HX_FIXEDSIZE_POOL_LIMIT_RESERVED = 0;

//固定大小池缓存数量不重置
static HX_CONSTEXPR int HX_FIXEDSIZE_POOL_CACHE_RESERVED = -1;

namespace HxLibs
{
	//固定大小池缓存大小不重置
	static HX_CONSTEXPR size_t FIXEDSIZE_POOL_SIZE_RESERVED = HX_FIXEDSIZE_POOL_SIZE_RESERVED;

	//固定大小池容量上限不限制
	static HX_CONSTEXPR int FIXEDSIZE_POOL_LIMIT_NONE = HX_FIXEDSIZE_POOL_LIMIT_NONE;
	//固定大小池容量上限不重置
	static HX_CONSTEXPR int FIXEDSIZE_POOL_LIMIT_RESERVED = HX_FIXEDSIZE_POOL_LIMIT_RESERVED;

	//固定大小池缓存数量不重置
	static HX_CONSTEXPR int FIXEDSIZE_POOL_CACHE_RESERVED = HX_FIXEDSIZE_POOL_CACHE_RESERVED;
}

#else //!__cplusplus

 //固定大小池缓存大小不重置
#define HX_FIXEDSIZE_POOL_SIZE_RESERVED 0

//固定大小池容量上限不限制
#define HX_FIXEDSIZE_POOL_LIMIT_NONE -1
//固定大小池容量上限不重置
#define HX_FIXEDSIZE_POOL_LIMIT_RESERVED 0

//固定大小池缓存数量不重置
#define HX_FIXEDSIZE_POOL_CACHE_RESERVED -1

#endif //__cplusplus

#endif //!HX_FIXEDSIZE_POOL_FLAGS

#endif //!HX_CORE_FIXEDSIZE_POOL_FLAGS_H
