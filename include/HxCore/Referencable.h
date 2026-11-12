/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */
#pragma once
#ifndef HX_CORE_REFERENCABLE_H
#define HX_CORE_REFERENCABLE_H

#ifndef HX_REFERENCABLE
#define HX_REFERENCABLE

#include "Interface.h"

 //表示可引用对象。
HX_INTERFACE_DECLARE(HxReferencable)
{
	//添加引用。
	HX_METHOD(void, addRef)(HX_THIS(HxReferencable)) HX_NOEXCEPT HX_PURE;
	//释放引用，如果删除对象则返回True。
	HX_METHOD(bool, release)(HX_THIS(HxReferencable)) HX_NOEXCEPT HX_PURE;
};

#ifdef __cplusplus
namespace HxLibs
{
	using Referencable = ::HxReferencable;
}
#define HX_REFERENCABLE_DECLARE_END(NAME)
#define HX_REFERENCABLE_DECLARE_BEGIN(NAME) HX_INTERFACE_DECLARE(NAME) : public ::HxReferencable
#define HX_REFERENCABLE_DECLARE_BEGIN_EX(NAME, UUID)\
struct NAME;\
template <> struct HxTypeid<NAME>\
{\
	HX_CONSTEXPR11_FUNC static const char* name() HX_NOEXCEPT { return #NAME; }\
	HX_CONSTEXPR11_FUNC static HxUuid uuid() HX_NOEXCEPT { return HxUuid(UUID); }\
};\
HX_INTERFACE_DECLARE(NAME) : public ::HxReferencable
#else //!__cplusplus
#define HX_REFERENCABLE_METHODS(NAME)\
HX_METHOD(void, addRef)(HX_THIS(NAME)) HX_NOEXCEPT HX_PURE;\
HX_METHOD(bool, release)(HX_THIS(NAME)) HX_NOEXCEPT HX_PURE;
#define HX_REFERENCABLE_DECLARE_END(NAME) };
#define HX_REFERENCABLE_DECLARE_BEGIN(NAME) HX_INTERFACE_DECLARE(NAME){ HX_REFERENCABLE_METHODS(NAME) struct
#define HX_REFERENCABLE_DECLARE_BEGIN_EX(NAME, UUID) HX_INTERFACE_DECLARE(NAME){ HX_REFERENCABLE_METHODS(NAME) struct
#endif //!__cplusplus

#endif //!HX_REFERENCABLE

#endif //!HX_CORE_REFERENCABLE_H
