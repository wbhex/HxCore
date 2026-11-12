/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */
#pragma once
#ifndef HX_CORE_REFERENCE_H
#define HX_CORE_REFERENCE_H

#ifndef HX_REFERENCE
#define HX_REFERENCE
#include "ObjRef.h"
#include "Referencable.h"

#ifdef __cplusplus
namespace HxLibs
{
	//声明引用对象
	template <typename T>
	class Reference;
	//声明只读引用对象
	template <typename T>
	class ConstReference;

	//表示引用对象
	template <typename T>
	class Reference
	{
		template <typename T2>
		friend class Reference;
		template <typename T2>
		friend class ConstReference;

	private:
		T* m_ptr;
		ObjRef<Referencable> m_ref;

	public:
		//构造函数
		HX_INLINE Reference() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_INLINE Reference(T* ptr, ObjRef<Referencable>&& ref) HX_NOEXCEPT
			: m_ptr(ptr), m_ref(::std::move(ref))
		{
			//NOTHING
		}
		HX_INLINE Reference(T* ptr, const ObjRef<Referencable>& ref) HX_NOEXCEPT
			: m_ptr(ptr), m_ref(ref)
		{
			//NOTHING
		}
		HX_INLINE Reference(const Reference& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(other.m_ref)
		{
			//NOTHING
		}
		HX_INLINE Reference(Reference&& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(::std::move(other.m_ref))
		{
			other.m_ptr = nullptr;
		}
		//赋值操作
		HX_INLINE Reference& operator=(const Reference& other) HX_NOEXCEPT
		{
			if (&other != this)
			{
				m_ptr = other.m_ptr;
				m_ref = other.m_ref;
			}
			return *this;
		}
		HX_INLINE Reference& operator=(Reference&& other) HX_NOEXCEPT
		{
			if (&other != this)
			{
				m_ptr = other.m_ptr;
				m_ref = ::std::move(other.m_ref);
				other.m_ptr = nullptr;
			}
			return *this;
		}

		//获取指针对象。
		HX_INLINE operator T* () const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//判断是否有效。
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}

		//执行交换操作。
		HX_INLINE void swap(Reference& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_ptr, other.m_ptr);
				m_ref.swap(other.m_ref);
			}
		}

		//获取指针对象。
		HX_INLINE T* operator->() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//判断是否有效。
		HX_INLINE bool valid() const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}
		//判断是否无效。
		HX_INLINE bool invalid() const HX_NOEXCEPT
		{
			return m_ptr == nullptr;
		}

		//获取目标对象。
		HX_INLINE T* target() const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//获取引用对象。
		HX_INLINE const ObjRef<Referencable>& refItem() const HX_NOEXCEPT
		{
			return m_ref;
		}
	};
	//表示只读引用对象
	template <typename T>
	class ConstReference
	{
		template <typename T2>
		friend class Reference;
		template <typename T2>
		friend class ConstReference;

	private:
		const T* m_ptr;
		ObjRef<Referencable> m_ref;

	public:
		//构造函数
		HX_INLINE ConstReference() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_INLINE ConstReference(const T* ptr, ObjRef<Referencable>&& ref) HX_NOEXCEPT
			: m_ptr(ptr), m_ref(::std::move(ref))
		{
			//NOTHING
		}
		HX_INLINE ConstReference(const T* ptr, const ObjRef<Referencable>& ref) HX_NOEXCEPT
			: m_ptr(ptr), m_ref(ref)
		{
			//NOTHING
		}
		HX_INLINE ConstReference(const Reference<T>& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(other.m_ref)
		{
			//NOTHING
		}
		HX_INLINE ConstReference(Reference<T>&& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(::std::move(other.m_ref))
		{
			other.m_ptr = nullptr;
		}
		HX_INLINE ConstReference(const ConstReference& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(other.m_ref)
		{
			//NOTHING
		}
		HX_INLINE ConstReference(ConstReference&& other) HX_NOEXCEPT
			: m_ptr(other.m_ptr), m_ref(::std::move(other.m_ref))
		{
			other.m_ptr = nullptr;
		}
		//赋值操作
		HX_INLINE ConstReference& operator=(const Reference<T>& other) HX_NOEXCEPT
		{
			m_ptr = other.m_ptr;
			m_ref = other.m_ref;
			return *this;
		}
		HX_INLINE ConstReference& operator=(Reference<T>&& other) HX_NOEXCEPT
		{
			m_ptr = other.m_ptr;
			m_ref = ::std::move(other.m_ref);
			other.m_ptr = nullptr;
			return *this;
		}
		HX_INLINE ConstReference& operator=(const ConstReference& other) HX_NOEXCEPT
		{
			if (&other != this)
			{
				m_ptr = other.m_ptr;
				m_ref = other.m_ref;
			}
			return *this;
		}
		HX_INLINE ConstReference& operator=(ConstReference&& other) HX_NOEXCEPT
		{
			if (&other != this)
			{
				m_ptr = other.m_ptr;
				m_ref = ::std::move(other.m_ref);
				other.m_ptr = nullptr;
			}
			return *this;
		}

		//获取指针对象。
		HX_INLINE operator const T* () const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//判断是否有效。
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}

		//执行交换操作。
		HX_INLINE void swap(ConstReference& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				::std::swap(m_ptr, other.m_ptr);
				m_ref.swap(other.m_ref);
			}
		}

		//获取指针对象。
		HX_INLINE const T* operator->() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//判断是否有效。
		HX_INLINE bool valid() const HX_NOEXCEPT
		{
			return m_ptr != nullptr;
		}
		//判断是否无效。
		HX_INLINE bool invalid() const HX_NOEXCEPT
		{
			return m_ptr == nullptr;
		}

		//获取目标对象。
		HX_INLINE const T* target() const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//获取引用对象。
		HX_INLINE const ObjRef<Referencable>& refItem() const HX_NOEXCEPT
		{
			return m_ref;
		}
	};
}
template <typename T>
using HxReference = ::HxLibs::Reference<T>;
template <typename T>
using HxConstReference = ::HxLibs::ConstReference<T>;
#else //!__cplusplus
#define HX_REFERENCE_DEFINE(NAME, TYPE)\
typedef struct NAME\
{\
	TYPE* target;\
	HxReferencable* refItem;\
} NAME;
#define HX_CONST_REFERENCE_DEFINE(NAME, TYPE)\
typedef struct NAME\
{\
	const TYPE* target;\
	HxReferencable* refItem;\
} NAME;
#endif //!__cplusplus
#endif //!HX_REFERENCE

#endif //!HX_CORE_REFERENCE_H
