/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_OBJECT_H
#define HX_OBJECT_H

#include "Referencable.h"

 //前向声明。
HX_INTERFACE_FORWARD_DECLARE(HxObject);

//表示对象枚举处理器。
HX_INTERFACE_DECLARE(HxObjectEnumHandler)
{
	//当开始时调用此方法。
	HX_METHOD(void, onBegin)(HX_THIS_(HxObjectEnumHandler) size_t count) HX_NOEXCEPT HX_PURE;
	//当报告时调用此方法。
	HX_METHOD(void, onItem)(HX_THIS_(HxObjectEnumHandler) HX_INTERFACE_FORWARD(HxObject)*, const HxUuid*) HX_NOEXCEPT HX_PURE;
	//当结束时调用此方法。
	HX_METHOD(void, onEnd)(HX_THIS(HxObjectEnumHandler)) HX_NOEXCEPT HX_PURE;
};
//定义对象枚举对象接口标识 {C50B8717-89C5-4BC2-8E1F-25EE11B14CEA}
#define HX_OBJECT_ENUM_PRINCIPAL 0xc50b8717, 0x89c5, 0x4bc2, 0x8e, 0x1f, 0x25, 0xee, 0x11, 0xb1, 0x4c, 0xea
//表示对象枚举对象接口。
HX_INTERFACE_DECLARE_EX(HxObjectEnumPrincipal, HX_OBJECT_ENUM_PRINCIPAL)
{
	//枚举所有对象。
	HX_METHOD(void, enumerate)(HX_CONST_THIS_(HxObjectEnumPrincipal) HxObjectEnumHandler*) HX_CONST HX_NOEXCEPT HX_PURE;

#ifdef __cplusplus
	//枚举所有对象。
	HX_INLINE void enumerate(HxObjectEnumHandler & handler) const HX_NOEXCEPT
	{
		enumerate(&handler);
	}
#endif //__cplusplus
};

//表示工作对象。
HX_REFERENCABLE_DECLARE_BEGIN(HxObject)
{
	//检索指定标识的对象（不会改变引用计数）。
	HX_METHOD(HxObject*, queryObject)(HX_CONST_THIS_(HxObject) const HxUuid*) HX_CONST HX_NOEXCEPT HX_PURE;
	//检索指标标识的接口（接口不包含引用计数，必须依赖于某个对象）。
	HX_METHOD(void*, queryInterface)(HX_CONST_THIS_(HxObject) const HxUuid*) HX_CONST HX_NOEXCEPT HX_PURE;

#ifdef __cplusplus
	//检索指定标识的对象（不会改变引用计数）。
	HX_INLINE HxObject* queryObject(const HxUuid & uuid) const HX_NOEXCEPT
	{
		return queryObject(&uuid);
	}
	//检索指标标识的接口（接口不包含引用计数，必须依赖于某个对象）。
	HX_INLINE void* queryInterface(const HxUuid & uuid) const HX_NOEXCEPT
	{
		return queryInterface(&uuid);
	}
	//检索指标标识的接口（接口不包含引用计数，必须依赖于某个对象）。
	template<typename T>
	HX_INLINE T* queryInterface() const HX_NOEXCEPT
	{
		return reinterpret_cast<T*>(queryInterface(HxTypeid<T>::uuid()));
	}

#ifndef HX_OBJECT_SIMPLE

	//枚举所有对象。
	HX_INLINE void enumObjects(HxObjectEnumHandler & handler) const HX_NOEXCEPT
	{
		auto iface = queryInterface<HxObjectEnumPrincipal>();
		if (iface != nullptr)
		{
			iface->enumerate(&handler);
		}
	}
	//枚举所有对象。
	HX_INLINE void enumObjects(HxObjectEnumHandler * handler) const HX_NOEXCEPT
	{
		enumObjects(*handler);
	}

	//枚举所有接口。
	HX_INLINE void enumInterfaces(HxInterfaceEnumHandler & handler) const HX_NOEXCEPT
	{
		auto iface = queryInterface<HxInterfaceEnumPrincipal>();
		if (iface != nullptr)
		{
			iface->enumerate(&handler);
		}
	}
	//枚举所有接口。
	HX_INLINE void enumInterfaces(HxInterfaceEnumHandler * handler) const HX_NOEXCEPT
	{
		enumInterfaces(*handler);
	}

#endif //!HX_OBJECT_SIMPLE
#endif //__cplusplus
};
HX_REFERENCABLE_DECLARE_END(HxObject)

//添加对象引用。
HX_INLINE void HxObject_addRef(HxObject* obj) HX_NOEXCEPT
{
	HX_CALL(obj, HxObject, addRef)(HX_IFACE(obj, HxObject));
}
//释放对象引用，如果删除对象则返回True。
HX_INLINE bool HxObject_release(HxObject* obj) HX_NOEXCEPT
{
	return HX_CALL(obj, HxObject, release)(HX_IFACE(obj, HxObject));
}
//查询指定标识的对象（不会改变引用计数）。
HX_INLINE HxObject* HxObject_queryObject(const HxObject* obj, const HxUuid* uuid) HX_NOEXCEPT
{
	return HX_CALL(obj, const HxObject, queryObject)(HX_IFACE_(obj, const HxObject) uuid);
}
//查询指定标识的接口（接口不包含引用计数，必须依赖于某个对象）。
HX_INLINE void* HxObject_queryInterface(const HxObject* obj, const HxUuid* uuid) HX_NOEXCEPT
{
	return HX_CALL(obj, const HxObject, queryInterface)(HX_IFACE_(obj, const HxObject) uuid);
}

//枚举所有对象。
HX_INLINE void HxObject_enumObjects(const HxObject* obj, HxObjectEnumHandler* handler) HX_NOEXCEPT
{
	HxUuid uuid = { HX_OBJECT_ENUM_PRINCIPAL };
	void* iface = HxObject_queryInterface(obj, &uuid);
	if (iface)
	{
		HX_CALL(iface, HxObjectEnumPrincipal, enumerate)(HX_IFACE_(iface, HxObjectEnumPrincipal) handler);
	}
}
//枚举所有接口。
HX_INLINE void HxObject_enumInterfaces(const HxObject* obj, HxInterfaceEnumHandler* handler) HX_NOEXCEPT
{
	HxUuid uuid = { HX_INTERFACE_ENUM_PRINCIPAL };
	void* iface = HxObject_queryInterface(obj, &uuid);
	if (iface)
	{
		HX_CALL(iface, HxInterfaceEnumPrincipal, enumerate)(HX_IFACE_(iface, HxInterfaceEnumPrincipal) handler);
	}
}

#endif //!HX_OBJECT_H
