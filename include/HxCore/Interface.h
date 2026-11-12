/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_INTERFACE_H
#define HX_INTERFACE_H

#include "Typeid.h"

#ifdef __cplusplus
#	define HX_THIS(NAME)
#	define HX_THIS_(NAME)
#	define HX_CONST_THIS(NAME)
#	define HX_CONST_THIS_(NAME)
#	define HX_CONST const
#	define HX_PURE = 0
#	define HX_METHOD(TYPE,NAME) virtual TYPE HX_STDCALL NAME
#	define HX_METHOD_VOID(NAME) virtual void HX_STDCALL NAME
#if HX_WINDOWS
#	define HX_IFACE_DECLARE(NAME) struct __declspec(novtable) NAME
#else //!HX_WINDOWS
#	define HX_IFACE_DECLARE(NAME) struct NAME
#endif //HX_WINDOWS
#	define HX_CALL(IFACE,TYPE,NAME) (reinterpret_cast<TYPE*>(IFACE))->NAME
#	define HX_IFACE(IFACE,TYPE)
#	define HX_IFACE_(IFACE,TYPE)
#	define HX_IFACE_FORWARD(NAME) NAME
#	define HX_IFACE_ALIAS(TSRC,TDST) using TDST = TSRC 
#else //__cplusplus
#	define HX_THIS(NAME) struct NAME*
#	define HX_THIS_(NAME) struct NAME*,
#	define HX_CONST_THIS(NAME) const struct NAME*
#	define HX_CONST_THIS_(NAME) const struct NAME*,
#	define HX_CONST
#	define HX_PURE
#	define HX_METHOD(TYPE,NAME) TYPE (HX_STDCALL *NAME)
#	define HX_METHOD_VOID(NAME) void (HX_STDCALL *NAME)
#	define HX_IFACE_DECLARE(NAME)	typedef struct NAME \
									{\
										struct NAME##Vtbl* pVtbl;\
									} NAME;\
									typedef struct NAME##Vtbl NAME##Vtbl;\
									struct NAME##Vtbl
#	define HX_CALL(IFACE,TYPE,NAME) ((TYPE*)(IFACE))->pVtbl->NAME
#	define HX_IFACE(IFACE,TYPE) ((TYPE*)(IFACE))
#	define HX_IFACE_(IFACE,TYPE) ((TYPE*)(IFACE)),
#	define HX_IFACE_FORWARD(NAME) struct NAME
#	define HX_IFACE_ALIAS(TSRC,TDST) typedef TSRC##Vtbl TDST##Vtbl; typedef TSRC TDST
#endif //__cplusplus

#define HX_IFACE_FORWARD_DECLARE(NAME) struct NAME

#define HX_INTERFACE(IFACE,TYPE) HX_IFACE(IFACE,TYPE)
#define HX_INTERFACE_(IFACE,TYPE) HX_IFACE_(IFACE,TYPE)
#define HX_INTERFACE_DECLARE(NAME) HX_IFACE_DECLARE(NAME)
#define HX_INTERFACE_ALIAS(TSRC, TDST) HX_IFACE_ALIAS(TSRC, TDST)
#define HX_INTERFACE_FORWARD(NAME) HX_IFACE_FORWARD(NAME)
#define HX_INTERFACE_FORWARD_DECLARE(NAME) HX_IFACE_FORWARD_DECLARE(NAME)

#ifdef __cplusplus
#define HX_INTERFACE_DECLARE_EX(NAME, UUID)\
struct NAME;\
template <> struct HxTypeid<NAME>\
{\
	HX_CONSTEXPR11_FUNC static const char* name() HX_NOEXCEPT { return #NAME; }\
	HX_CONSTEXPR11_FUNC static HxUuid uuid() HX_NOEXCEPT { return HxUuid(UUID); }\
};\
HX_INTERFACE_DECLARE(NAME)
#else //__cplusplus
#define HX_INTERFACE_DECLARE_EX(NAME, UUID) HX_INTERFACE_DECLARE(NAME)
#endif //__cplusplus

 //表示接口枚举处理器。
HX_INTERFACE_DECLARE(HxInterfaceEnumHandler)
{
	//当开始时调用此方法。
	HX_METHOD(void, onBegin)(HX_THIS_(HxInterfaceEnumHandler) size_t count) HX_NOEXCEPT HX_PURE;
	//当报告变量项时调用此方法。
	HX_METHOD(void, onItem)(HX_THIS_(HxInterfaceEnumHandler) void*, const HxUuid*) HX_NOEXCEPT HX_PURE;
	//当结束时调用此方法。
	HX_METHOD(void, onEnd)(HX_THIS(HxInterfaceEnumHandler)) HX_NOEXCEPT HX_PURE;
};

//定义接口枚举对象接口标识 {99883B3F-FD15-48E2-B896-990201BC14F2}
#define HX_INTERFACE_ENUM_PRINCIPAL 0x99883b3f, 0xfd15, 0x48e2, 0xb8, 0x96, 0x99, 0x2, 0x1, 0xbc, 0x14, 0xf2
//表示接口枚举对象接口。
HX_INTERFACE_DECLARE_EX(HxInterfaceEnumPrincipal, HX_INTERFACE_ENUM_PRINCIPAL)
{
	//枚举所有接口。
	HX_METHOD(void, enumerate)(HX_CONST_THIS_(HxInterfaceEnumPrincipal) HxInterfaceEnumHandler*) HX_CONST HX_NOEXCEPT HX_PURE;

#ifdef __cplusplus
	//枚举所有接口。
	HX_INLINE void enumerate(HxInterfaceEnumHandler & handler) const HX_NOEXCEPT
	{
		enumerate(&handler);
	}
#endif //__cplusplus
};

#ifdef __cplusplus
namespace HxLibs
{
	using InterfaceEnumHandler = ::HxInterfaceEnumHandler;
	using InterfaceEnumPrincipal = ::HxInterfaceEnumPrincipal;
}
#endif //__cplusplus

#endif //!HX_INTERFACE_H
