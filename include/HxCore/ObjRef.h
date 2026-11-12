/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_OBJREF_H
#define HX_OBJREF_H

#ifdef __cplusplus
#include <memory>
#include <type_traits>
#include "Defines.h"
namespace HxLibs
{
	//表示对象引用。
	template <typename T>
	class ObjRef
	{
		template <typename T2>
		friend class ::HxLibs::ObjRef;

	private:
		//内部指针。
		T* m_p;

	public:
		//初始为空引用。
		HX_INLINE ObjRef() HX_NOEXCEPT
			: m_p(nullptr)
		{
			//NOTHING
		}
		//初始化为指定指针对象引用。
		HX_INLINE ObjRef(T* p) HX_NOEXCEPT
			: m_p(p)
		{
			if (m_p != nullptr)
			{
				m_p->addRef();
			}
		}
		//初始化为空引用。
		HX_INLINE ObjRef(::std::nullptr_t) HX_NOEXCEPT
			: m_p(nullptr)
		{
			//NOTHING
		}
		//拷贝构造函数。
		HX_INLINE ObjRef(const ObjRef& other) HX_NOEXCEPT
			: m_p(other.m_p)
		{
			if (m_p != nullptr)
			{
				m_p->addRef();
			}
		}
		//移动构造函数。
		HX_INLINE ObjRef(ObjRef&& other) HX_NOEXCEPT
			: m_p(other.m_p)
		{
			other.m_p = nullptr;
		}

		//拷贝构造函数。
		template <typename T2>
		HX_INLINE ObjRef(const ObjRef<T2>& other) HX_NOEXCEPT
			: m_p(static_cast<T*>(other.m_p))
		{
			if (m_p != nullptr)
			{
				m_p->addRef();
			}
		}

		//析构函数。
		HX_INLINE ~ObjRef() HX_NOEXCEPT
		{
			if (m_p != nullptr)
			{
				m_p->release();
				m_p = nullptr;
			}
		}

		//获取指针对象。
		HX_INLINE operator T* () const HX_NOEXCEPT
		{
			return m_p;
		}

		//判断是否有效。
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return m_p != nullptr;
		}

		//获取指针对象。
		HX_INLINE T* operator->() const HX_NOEXCEPT
		{
			return m_p;
		}

		//执行交换操作。
		HX_INLINE void swap(ObjRef& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_p, other.m_p);
			}
		}

		//附加指定对象。
		HX_INLINE void attach(T* p) HX_NOEXCEPT
		{
			if (m_p != p)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = p;
			}
		}
		//分离指定对象。
		HX_INLINE T* detach() HX_NOEXCEPT
		{
			T* p = m_p;
			m_p = nullptr;
			return p;
		}

		//判断是否有效。
		HX_INLINE bool valid() const HX_NOEXCEPT
		{
			return m_p != nullptr;
		}
		//判断是否无效。
		HX_INLINE bool invalid() const HX_NOEXCEPT
		{
			return m_p == nullptr;
		}

		//获取目标对象。
		HX_INLINE T* target() const HX_NOEXCEPT
		{
			return m_p;
		}

		//获取共享指针对象。
		HX_INLINE::std::shared_ptr<T> share() HX_NOEXCEPT
		{
			if (m_p != nullptr)
			{
				m_p->addRef();
				try
				{
					return ::std::shared_ptr<T>(m_p, [](T* p) { p->release(); });
				}
				catch (...) //::std::bad_alloc
				{
					m_p->release();
				}
			}
			return ::std::shared_ptr<T>();
		}
		//获取共享指针对象。
		template <typename Alloc>
		HX_INLINE::std::shared_ptr<T> share(const Alloc& alloc) HX_NOEXCEPT
		{
			if (m_p != nullptr)
			{
				m_p->addRef();
				try
				{
					return ::std::shared_ptr<T>(m_p, [](T* p) { p->release(); }, alloc);
				}
				catch (...) //::std::bad_alloc
				{
					m_p->release();
				}
			}
			return ::std::shared_ptr<T>();
		}

		//赋值操作符。
		HX_INLINE ObjRef& operator=(::std::nullptr_t) HX_NOEXCEPT
		{
			if (m_p != nullptr)
			{
				m_p->release();
				m_p = nullptr;
			}
			return *this;
		}
		//赋值操作符。
		HX_INLINE ObjRef& operator=(T* other) HX_NOEXCEPT
		{
			if (m_p != other)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = other;
				if (m_p != nullptr)
				{
					m_p->addRef();
				}
			}
			return *this;
		}
		//赋值操作符。
		HX_INLINE ObjRef& operator=(const ObjRef& other) HX_NOEXCEPT
		{
			if (m_p != other.m_p)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = other.m_p;
				if (m_p != nullptr)
				{
					m_p->addRef();
				}
			}
			return *this;
		}
		//移动操作符。
		HX_INLINE ObjRef& operator=(ObjRef&& other) HX_NOEXCEPT
		{
			if (m_p != other.m_p)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = other.m_p;
				other.m_p = nullptr;
			}
			else if (&other != this)
			{
				other = nullptr;
			}
			return *this;
		}

		//赋值操作符。
		template <typename T2>
		HX_INLINE ObjRef& operator=(T2* other) HX_NOEXCEPT
		{
			T* p = static_cast<T*>(other);
			if (m_p != p)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = p;
				if (m_p != nullptr)
				{
					m_p->addRef();
				}
			}
			return *this;
		}
		//赋值操作符。
		template <typename T2>
		HX_INLINE ObjRef& operator=(const ObjRef<T2>& other) HX_NOEXCEPT
		{
			T* p = static_cast<T*>(other.m_p);
			if (m_p != p)
			{
				if (m_p != nullptr)
				{
					m_p->release();
				}
				m_p = p;
				if (m_p != nullptr)
				{
					m_p->addRef();
				}
			}
			return *this;
		}

		//等于操作符。
		HX_INLINE bool operator==(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_p == nullptr;
		}
		//等于操作符。
		HX_INLINE bool operator==(const T* other) const HX_NOEXCEPT
		{
			return m_p == other;
		}
		//等于操作符。
		HX_INLINE bool operator==(const ObjRef& other) const HX_NOEXCEPT
		{
			return m_p == other.m_p;
		}

		//等于操作符。
		template <typename T2>
		HX_INLINE bool operator==(const T2* other) const HX_NOEXCEPT
		{
			T* p = static_cast<const T*>(other);
			return m_p == p;
		}
		//等于操作符。
		template <typename T2>
		HX_INLINE bool operator==(const ObjRef<T2>& other) const HX_NOEXCEPT
		{
			T* p = static_cast<T*>(other.m_p);
			return m_p == p;
		}

		//不等于操作符。
		HX_INLINE bool operator!=(::std::nullptr_t) const HX_NOEXCEPT
		{
			return m_p != nullptr;
		}
		//不等于操作符。
		HX_INLINE bool operator!=(const T* other) const HX_NOEXCEPT
		{
			return m_p != other;
		}
		//不等于操作符。
		HX_INLINE bool operator!=(const ObjRef& other) const HX_NOEXCEPT
		{
			return m_p != other.m_p;
		}

		//不等于操作符。
		template <typename T2>
		HX_INLINE bool operator!=(const T2* other) const HX_NOEXCEPT
		{
			T* p = static_cast<const T*>(other);
			return m_p != p;
		}
		//不等于操作符。
		template <typename T2>
		HX_INLINE bool operator!=(const ObjRef<T2>& other) const HX_NOEXCEPT
		{
			T* p = static_cast<T*>(other.m_p);
			return m_p != p;
		}
	};

	//等于操作符。
	template <typename T>
	HX_INLINE bool operator==(const T* left, const ObjRef<T> right) HX_NOEXCEPT
	{
		return right == left;
	}
	//等于操作符。
	template <typename T>
	HX_INLINE bool operator==(::std::nullptr_t, const ObjRef<T>& value) HX_NOEXCEPT
	{
		return value == nullptr;
	}
	//等于操作符。
	template <typename T1, typename T2>
	HX_INLINE bool operator==(const T1* left, const ObjRef<T2> right) HX_NOEXCEPT
	{
		return right == left;
	}

	//不等于操作符。
	template <typename T>
	HX_INLINE bool operator!=(const T* left, const ObjRef<T> right) HX_NOEXCEPT
	{
		return right != left;
	}
	//不等于操作符。
	template <typename T>
	HX_INLINE bool operator!=(::std::nullptr_t, const ObjRef<T>& value) HX_NOEXCEPT
	{
		return value != nullptr;
	}
	//不等于操作符。
	template <typename T1, typename T2>
	HX_INLINE bool operator!=(const T1* left, const ObjRef<T2> right) HX_NOEXCEPT
	{
		return right != left;
	}
}
template <typename T>
using HxObjRef = ::HxLibs::ObjRef<T>;
#endif //__cplusplus

#endif //!HX_OBJREF_H
