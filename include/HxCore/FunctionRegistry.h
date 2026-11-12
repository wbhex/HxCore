/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FUNCTION_REGISTRY_H
#define HX_FUNCTION_REGISTRY_H

#include "Interface.h"

//定义函数登记标识 {6D903C69-FC39-43A3-8D46-B1556464611F}
#define HX_FUNCTION_REGISTRY 0x6d903c69, 0xfc39, 0x43a3, 0x8d, 0x46, 0xb1, 0x55, 0x64, 0x64, 0x61, 0x1f
//表示函数登记接口。
HX_INTERFACE_DECLARE_EX(HxFunctionRegistry, HX_FUNCTION_REGISTRY)
{
	//附加指定的函数。
	HX_METHOD(void, attach)(HX_THIS_(HxFunctionRegistry) const HxUuid*, void*) HX_NOEXCEPT HX_PURE;
	//取消附加指定的函数。
	HX_METHOD(bool, detach)(HX_THIS_(HxFunctionRegistry) const HxUuid*, void*) HX_NOEXCEPT HX_PURE;
};

#ifdef __cplusplus
namespace HxLibs
{
	using FunctionRegistry = ::HxFunctionRegistry;
}
#endif //__cplusplus

#endif //!HX_FUNCTION_REGISTRY_H
