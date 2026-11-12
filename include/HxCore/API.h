/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_API_H
#define HX_CORE_API_H

#include "Typeid.h"

#ifndef HX_CORE_API
#define HX_CORE_API
#endif //!HX_CORE_API

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	//获取指定标识的函数。
	HX_CORE_API void* HX_STDCALL HxCore_fquery(const HxUuid*) HX_NOEXCEPT;
	//获取指定标识的接口实例。
	HX_CORE_API void* HX_STDCALL HxCore_iquery(const HxUuid*) HX_NOEXCEPT;
#ifdef __cplusplus
} //extern "C"
#endif //__cplusplus

HX_INLINE void* HxCore_queryFunction(const HxUuid* uuid) HX_NOEXCEPT
{
	return HxCore_fquery(uuid);
}

HX_INLINE void* HxCore_queryInterface(const HxUuid* uuid) HX_NOEXCEPT
{
	return HxCore_iquery(uuid);
}

#ifdef __cplusplus
namespace HxLibs
{
	template<typename T>
	HX_INLINE T queryFunction(const Uuid& uuid) HX_NOEXCEPT
	{
		return reinterpret_cast<T>(HxCore_fquery(&uuid));
	}

	template<typename T>
	HX_INLINE T* queryInterface(const Uuid& uuid) HX_NOEXCEPT
	{
		return reinterpret_cast<T*>(HxCore_iquery(&uuid));
	}
	template<typename T>
	HX_INLINE T* queryInterface() HX_NOEXCEPT
	{
		return queryInterface<T>(Typeid<T>::uuid());
	}
}

template<typename T>
HX_INLINE T HxCore_queryFunction(const HxUuid& uuid) HX_NOEXCEPT
{
	return reinterpret_cast<T>(HxCore_fquery(&uuid));
}

template<typename T>
HX_INLINE T* HxCore_queryInterface(const HxUuid& uuid) HX_NOEXCEPT
{
	return reinterpret_cast<T*>(HxCore_iquery(&uuid));
}
template<typename T>
HX_INLINE T* HxCore_queryInterface() HX_NOEXCEPT
{
	return HxCore_queryInterface<T>(HxTypeid<T>::uuid());
}

#endif //__cplusplus

#endif //!HX_CORE_API_H
