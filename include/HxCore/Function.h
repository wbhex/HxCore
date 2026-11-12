/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FUNCTION_H
#define HX_FUNCTION_H

#include "Defines.h"

#ifndef HX_FUNCTION_DECLARE
#define HX_FUNCTION_DECLARE(RET, NAME, ARGS) typedef RET (HX_STDCALL* NAME) ARGS;
#endif //!HX_FUNCTION_DECLARE

#include "Interface.h"

//表示函数枚举处理器。
HX_INTERFACE_DECLARE(HxFunctionEnumHandler)
{
	//当开始时调用此方法。
	HX_METHOD(void, onBegin)(HX_THIS_(HxFunctionEnumHandler) size_t count) HX_NOEXCEPT HX_PURE;
	//当报告变量项时调用此方法。
	HX_METHOD(void, onItem)(HX_THIS_(HxFunctionEnumHandler) void*, const HxUuid*) HX_NOEXCEPT HX_PURE;
	//当结束时调用此方法。
	HX_METHOD(void, onEnd)(HX_THIS(HxFunctionEnumHandler)) HX_NOEXCEPT HX_PURE;
};

//定义函数枚举对象接口标识 {93CCEC98-32A0-485B-8C86-E85ADA1517D5}
#define HX_FUNCTION_ENUM_PRINCIPAL 0x93ccec98, 0x32a0, 0x485b, 0x8c, 0x86, 0xe8, 0x5a, 0xda, 0x15, 0x17, 0xd5
//表示函数枚举对象接口。
HX_INTERFACE_DECLARE_EX(HxFunctionEnumPrincipal, HX_FUNCTION_ENUM_PRINCIPAL)
{
	//枚举所有函数。
	HX_METHOD(void, enumerate)(HX_CONST_THIS_(HxFunctionEnumPrincipal) HxFunctionEnumHandler*) HX_CONST HX_NOEXCEPT HX_PURE;

#ifdef __cplusplus
	//枚举所有函数。
	HX_INLINE void enumerate(HxFunctionEnumHandler & handler) const HX_NOEXCEPT
	{
		enumerate(&handler);
	}
#endif //__cplusplus
};

#ifdef __cplusplus
namespace HxLibs
{
	using FunctionEnumHandler = ::HxFunctionEnumHandler;
	using FunctionEnumPrincipal = ::HxFunctionEnumPrincipal;
}
#endif //__cplusplus

#endif //!HX_FUNCTION_H
