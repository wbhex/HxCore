/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_INTERFACE_REGISTRY_H
#define HX_INTERFACE_REGISTRY_H

#include "Interface.h"

//定义接口登记标识 {4FE789C7-43DC-4DAD-8A98-F3D337471964}
#define HX_INTERFACE_REGISTRY 0x4fe789c7, 0x43dc, 0x4dad, 0x8a, 0x98, 0xf3, 0xd3, 0x37, 0x47, 0x19, 0x64
//表示接口登记接口。
HX_INTERFACE_DECLARE_EX(HxInterfaceRegistry, HX_INTERFACE_REGISTRY)
{
	//附加指定的接口。
	HX_METHOD(void, attach)(HX_THIS_(HxInterfaceRegistry) const HxUuid*, void*) HX_NOEXCEPT HX_PURE;
	//取消附加指定的接口。
	HX_METHOD(bool, detach)(HX_THIS_(HxInterfaceRegistry) const HxUuid*, void*) HX_NOEXCEPT HX_PURE;
};

#ifdef __cplusplus
namespace HxLibs
{
	using InterfaceRegistry = ::HxInterfaceRegistry;
}
#endif //__cplusplus

#endif //!HX_INTERFACE_REGISTRY_H
