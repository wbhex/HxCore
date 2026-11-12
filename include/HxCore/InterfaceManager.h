/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_INTERFACE_MANAGER_H
#define HX_INTERFACE_MANAGER_H

#ifdef __cplusplus
#include "LockGuard.h"
#include "Uncopyable.h"
#include "InterfaceRegistry.h"
#include "Containers/HashMap.h"

namespace HxLibs
{
	//表示接口集合类。
	class InterfaceManager : Uncopyable
		, public InterfaceRegistry
		, public InterfaceEnumPrincipal
	{
		using Locker = ::HxLibs::AtomicLocker;
		using LockGuard = ::HxLibs::SpinLockGuard<Locker>;

	private:
		mutable Locker m_mutex;
		HashMap<Uuid, void*> m_items;

	protected:
		//添加指定的接口。
		template <typename T>
		HX_INLINE void add(T* item) HX_NOEXCEPT
		{
			m_items.insert(::std::pair<Uuid, void*>(Typeid<T>::uuid(), item));
		}
		HX_INLINE void add(const Uuid& uuid, void* item) HX_NOEXCEPT
		{
			m_items.insert(::std::pair<Uuid, void*>(uuid, item));
		}
		//移除指定的接口。
		template <typename T>  
		HX_INLINE bool remove(T* item) HX_NOEXCEPT
		{
			auto it = m_items.find(Typeid<T>::uuid());
			if (it != m_items.end())
			{
				if (it->second == item)
				{
					m_items.erase(it);
					return true;
				}
			}
			return false;
		}
		HX_INLINE bool remove(const Uuid& uuid, void* item) HX_NOEXCEPT
		{
			auto it = m_items.find(uuid);
			if (it != m_items.end())
			{
				if (it->second == item)
				{
					m_items.erase(it);
					return true;
				}
			}
			return false;
		}

	public:
		//初始化接口集合类，根据参数决定是否注册实现接口。
		HX_INLINE explicit InterfaceManager(bool init = false) HX_NOEXCEPT
		{
			if (init)
			{
				add(static_cast<HxInterfaceRegistry*>(this));
				add(static_cast<HxInterfaceEnumPrincipal*>(this));
			}
		}

		//查询指定接口。
		HX_INLINE void* query(const Uuid& uuid) const HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			auto it = m_items.find(uuid);
			if (it != m_items.end())
			{
				return it->second;
			}
			return nullptr;
		}
		//查询指定接口。
		HX_INLINE void* query(const Uuid* uuid) const HX_NOEXCEPT
		{
			if (uuid != nullptr)
			{
				return query(*uuid);
			}
			return nullptr;
		}
		//查询指定接口。
		template <typename T>
		HX_INLINE T* query() const HX_NOEXCEPT
		{
			return reinterpret_cast<T*>(query(Typeid<T>::uuid()));
		}

		//附加指定的接口。
		template <typename T>
		HX_INLINE void attach(T* item) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			add(item);
		}
		//附加指定的接口。
		HX_INLINE void attach(const Uuid& uuid, void* item) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			add(uuid, item);
		}
		//附加指定的接口。
		HX_INLINE virtual void HX_STDCALL attach(const Uuid* uuid, void* item) HX_NOEXCEPT override
		{
			LockGuard lck(m_mutex);
			add(*uuid, item);
		}

		//取消附加指定接口。
		template <typename T>
		HX_INLINE bool detach(T* item) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			return remove(item);
		}
		//取消附加指定接口。
		HX_INLINE bool detach(const Uuid& uuid, void* item) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			return remove(uuid, item);
		}
		//取消附加指定接口。
		HX_INLINE virtual bool HX_STDCALL detach(const Uuid* uuid, void* item) HX_NOEXCEPT override
		{
			LockGuard lck(m_mutex);
			return remove(*uuid, item);
		}

		//枚举所有接口。
		HX_INLINE void HX_STDCALL enumerate(InterfaceEnumHandler& handler) const HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			handler.onBegin(m_items.size());
			if (m_items.size() > 0)
			{
				for (auto it = m_items.begin(); it != m_items.end(); ++it)
				{
					handler.onItem(it->second, &(it->first));
				}
			}
			handler.onEnd();
		}
		//枚举所有接口。
		HX_INLINE virtual void HX_STDCALL enumerate(InterfaceEnumHandler* handler) const HX_NOEXCEPT override
		{
			LockGuard lck(m_mutex);
			handler->onBegin(m_items.size());
			if (m_items.size() > 0)
			{
				for (auto it = m_items.begin(); it != m_items.end(); ++it)
				{
					handler->onItem(it->second, &(it->first));
				}
			}
			handler->onEnd();
		}
	};
}
#endif //__cplusplus

#endif //HX_INTERFACE_MANAGER_H
