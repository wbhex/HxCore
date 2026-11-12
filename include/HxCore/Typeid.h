/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_TYPEID_H
#define HX_TYPEID_H

#include "Uuid.h"
#ifdef __cplusplus
template <typename T>
struct HxTypeid
{
	//HX_CONSTEXPR11_FUNC static const char* name() HX_NOEXCEPT;
	//HX_CONSTEXPR11_FUNC static HxUuid uuid() HX_NOEXCEPT;
};
namespace HxLibs
{
	template <typename T>
	using Typeid = HxTypeid<T>;
}
#define HX_TYPEID_DEFINE(NAME, UUID)\
template <> struct HxTypeid<NAME>\
{\
	HX_CONSTEXPR11_FUNC static const char* name() HX_NOEXCEPT { return NAME; }\
	HX_CONSTEXPR11_FUNC static HxUuid uuid() HX_NOEXCEPT { return HxUuid(UUID); }\
};
#else //__cplusplus
#define HX_TYPEID_DEFINE(NAME, UUID)
#endif //__cplusplus

#endif // !HX_TYPEID_H
