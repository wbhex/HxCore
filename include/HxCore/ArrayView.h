/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_ARRAY_VIEW_H
#define HX_CORE_ARRAY_VIEW_H

#include "Sequence.h"

#ifdef __cplusplus
#ifndef HX_ARRAY_VIEW
#define HX_ARRAY_VIEW

namespace HxLibs
{
	template <typename T>
	class __ArrayView__
	{
		HX_STATIC_ASSERT((::std::is_pointer<T>::value), "::HxLibs::__ArrayView__<T> Type parameter must be pointer.");

	protected:
		union
		{
			struct
			{
				T m_ptr;
				size_t m_size;
			};
			Sequence<T> m_seqs;
		};

	public:
		//获取内部指针。
		HX_CONSTEXPR11_FUNC T ptr() const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//获取内部指针。
		HX_CONSTEXPR11_FUNC T data() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//获取数组大小。
		HX_CONSTEXPR11_FUNC size_t size() const HX_NOEXCEPT
		{
			return m_size;
		}
		//获取数组大小。
		HX_CONSTEXPR11_FUNC size_t count() const HX_NOEXCEPT
		{
			return m_size;
		}
		//获取数组大小。
		HX_CONSTEXPR11_FUNC size_t length() const HX_NOEXCEPT
		{
			return m_size;
		}

		//获取数组是否为空。
		HX_CONSTEXPR11_FUNC bool empty() const HX_NOEXCEPT
		{
			return m_size == 0;
		}

		//默认构造函数。
		HX_CONSTEXPR11_FUNC __ArrayView__() HX_NOEXCEPT
			: m_seqs(nullptr, 0)
		{
			//NOTHING
		}
		//使用指定的内容和大小初始化。
		HX_CONSTEXPR11_FUNC __ArrayView__(T ptr, size_t size) HX_NOEXCEPT
			: m_seqs(ptr, size)
		{
			//NOTHING
		}
		//使用序列初始化。
		HX_CONSTEXPR11_FUNC __ArrayView__(const Sequence<T>& other) HX_NOEXCEPT
			: m_seqs(other)
		{
			//NOTHING
		}

		//重置数组为空。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			m_ptr = nullptr;
			m_size = 0;
		}
		//重置数组为指定内容。
		HX_INLINE void reset(T ptr, size_t size) HX_NOEXCEPT
		{
			m_ptr = ptr;
			m_size = size;
		}
		//重置数组为指定内容。
		HX_INLINE void reset(const Sequence<T>& other) HX_NOEXCEPT
		{
			m_ptr = other.ptr;
			m_size = other.size;
		}

		//重置数组为空。
		HX_INLINE void operator= (::std::nullptr_t) HX_NOEXCEPT
		{
			m_ptr = ptr;
			m_size = size;
		}

		//获取指针。
		HX_INLINE T operator()() const HX_NOEXCEPT
		{
			return m_ptr;
		}

		//转换为指针。
		HX_INLINE operator T () const HX_NOEXCEPT
		{
			return m_ptr;
		}
		//转换为序列。
		HX_INLINE operator const Sequence<T>& () const HX_NOEXCEPT
		{
			return m_seqs;
		}

		//获取内部指针（仅限高级使用）。
		HX_INLINE explicit operator T& () HX_NOEXCEPT
		{
			return m_ptr;
		}
	};

	//表示数组视图（不持有）
	template <typename T>
	class ArrayView : public __ArrayView__<T*>
	{
		using _MyBase = __ArrayView__<T*>;

	public:
		HX_CONSTEXPR11_FUNC ArrayView() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(T* ptr, size_t size) HX_NOEXCEPT
			: _MyBase(ptr, size)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(T* begin, T* end) HX_NOEXCEPT
			: _MyBase(begin, end - begin)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(T* ptr, size_t offset, size_t size) HX_NOEXCEPT
			: _MyBase(ptr + offset, size)
		{
			//NOTHING
		}

		HX_INLINE ArrayView(const __ArrayView__<T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayView& operator=(const __ArrayView__<T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayView(const Sequence<T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayView& operator=(const Sequence<T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr;
			_MyBase::m_size = other.size;
			return *this;
		}

		HX_INLINE T* operator+ (size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr + pos;
		}

		HX_INLINE T& at(size_t pos) HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}
		HX_INLINE const T& at(size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}

		HX_INLINE T& operator[] (size_t pos) HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}
		HX_INLINE const T& operator[] (size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}

		HX_INLINE T* begin() const HX_NOEXCEPT
		{
			return _MyBase::m_ptr;
		}
		HX_INLINE T* end() const HX_NOEXCEPT
		{
			return _MyBase::m_ptr + _MyBase::m_size;
		}

		HX_INLINE T& front() HX_NOEXCEPT
		{
			return *_MyBase::m_ptr;
		}
		HX_INLINE const T& front() const HX_NOEXCEPT
		{
			return *_MyBase::m_ptr;
		}

		HX_INLINE T& back() HX_NOEXCEPT
		{
			return *(_MyBase::m_ptr + _MyBase::m_size - 1);
		}
		HX_INLINE const T& back() const HX_NOEXCEPT
		{
			return *(_MyBase::m_ptr + _MyBase::m_size - 1);
		}

		HX_INLINE ArrayView extract(size_t pos) const HX_NOEXCEPT
		{
			return ArrayView(_MyBase::m_ptr + pos, _MyBase::m_size - pos);
		}
		HX_INLINE ArrayView extract(size_t pos, size_t cnt) const HX_NOEXCEPT
		{
			return ArrayView(_MyBase::m_ptr + pos, cnt);
		}
	};
	//表示只读数组视图（不持有）
	template <typename T>
	class ArrayConstView : public __ArrayView__<const T*>
	{
		using _MyBase = __ArrayView__<const T*>;

	public:
		HX_CONSTEXPR11_FUNC ArrayConstView() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const T* ptr, size_t size) HX_NOEXCEPT
			: _MyBase(ptr, size)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const T* begin, const T* end) HX_NOEXCEPT
			: _MyBase(begin, end - begin)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const T* ptr, size_t offset, size_t size) HX_NOEXCEPT
			: _MyBase(ptr + offset, size)
		{
			//NOTHING
		}

		HX_INLINE ArrayConstView(const __ArrayView__<const T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const __ArrayView__<const T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayConstView(const __ArrayView__<T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const __ArrayView__<T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayConstView(const Sequence<const T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const Sequence<const T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr;
			_MyBase::m_size = other.size;
			return *this;
		}

		HX_INLINE ArrayConstView(const Sequence<T*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const Sequence<T*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr;
			_MyBase::m_size = other.size;
			return *this;
		}

		HX_INLINE const T* operator+ (size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr + pos;
		}

		HX_INLINE const T& at(size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}
		HX_INLINE const T& operator[] (size_t pos) const HX_NOEXCEPT
		{
			return _MyBase::m_ptr[pos];
		}

		HX_INLINE const T* begin() const HX_NOEXCEPT
		{
			return _MyBase::m_ptr;
		}
		HX_INLINE const T* end() const HX_NOEXCEPT
		{
			return _MyBase::m_ptr + _MyBase::m_size;
		}

		HX_INLINE const T& front() const HX_NOEXCEPT
		{
			return *_MyBase::m_ptr;
		}
		HX_INLINE const T& back() const HX_NOEXCEPT
		{
			return *(_MyBase::m_ptr + _MyBase::m_size - 1);
		}

		HX_INLINE ArrayConstView extract(size_t pos) const HX_NOEXCEPT
		{
			return ArrayConstView(_MyBase::m_ptr + pos, _MyBase::m_size - pos);
		}
		HX_INLINE ArrayConstView extract(size_t pos, size_t cnt) const HX_NOEXCEPT
		{
			return ArrayConstView(_MyBase::m_ptr + pos, cnt);
		}
	};

	//表示数组视图（不持有）
	template <>
	class ArrayView<void> : public __ArrayView__<void*>
	{
		using _MyBase = __ArrayView__<void*>;

	public:
		HX_CONSTEXPR11_FUNC ArrayView() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(void* ptr, size_t size) HX_NOEXCEPT
			: _MyBase(ptr, size)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(void* begin, void* end) HX_NOEXCEPT
			: _MyBase(begin, (char*)end - (char*)begin)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayView(void* ptr, size_t offset, size_t size) HX_NOEXCEPT
			: _MyBase((char*)ptr + offset, size)
		{
			//NOTHING
		}

		HX_INLINE ArrayView(const __ArrayView__<void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayView& operator=(const __ArrayView__<void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayView(const Sequence<void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayView& operator=(const Sequence<void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr;
			_MyBase::m_size = other.size;
			return *this;
		}

		HX_INLINE void* operator+ (size_t pos) const HX_NOEXCEPT
		{
			return (char*)_MyBase::m_ptr + pos;
		}

		HX_INLINE ArrayView extract(size_t pos) const HX_NOEXCEPT
		{
			return ArrayView((void*)((char*)_MyBase::m_ptr + pos), _MyBase::m_size - pos);
		}
		HX_INLINE ArrayView extract(size_t pos, size_t cnt) const HX_NOEXCEPT
		{
			return ArrayView((void*)((char*)_MyBase::m_ptr + pos), cnt);
		}
	};
	//表示只读数组视图（不持有）
	template <>
	class ArrayConstView<void> : public __ArrayView__<const void*>
	{
		using _MyBase = __ArrayView__<const void*>;

	public:
		HX_CONSTEXPR11_FUNC ArrayConstView() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const void* ptr, size_t size) HX_NOEXCEPT
			: _MyBase(ptr, size)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const void* begin, const void* end) HX_NOEXCEPT
			: _MyBase(begin, (const char*)end - (const char*)begin)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC ArrayConstView(const void* ptr, size_t offset, size_t size) HX_NOEXCEPT
			: _MyBase((const char*)ptr + offset, size)
		{
			//NOTHING
		}

		HX_INLINE ArrayConstView(const __ArrayView__<const void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const __ArrayView__<const void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayConstView(const __ArrayView__<void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr(), other.size())
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const __ArrayView__<void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr();
			_MyBase::m_size = other.size();
			return *this;
		}

		HX_INLINE ArrayConstView(const Sequence<const void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const Sequence<const void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_seqs = other;
			return *this;
		}

		HX_INLINE ArrayConstView(const Sequence<void*>& other) HX_NOEXCEPT
			: _MyBase(other.ptr, other.size)
		{
			//NOTHING
		}
		HX_INLINE ArrayConstView& operator=(const Sequence<void*>& other) HX_NOEXCEPT
		{
			_MyBase::m_ptr = other.ptr;
			_MyBase::m_size = other.size;
			return *this;
		}

		HX_INLINE const void* operator+ (size_t pos) const HX_NOEXCEPT
		{
			return (const char*)_MyBase::m_ptr + pos;
		}

		HX_INLINE ArrayConstView extract(size_t pos) const HX_NOEXCEPT
		{
			return ArrayConstView((const void*)((const char*)_MyBase::m_ptr + pos), _MyBase::m_size - pos);
		}
		HX_INLINE ArrayConstView extract(size_t pos, size_t cnt) const HX_NOEXCEPT
		{
			return ArrayConstView((const void*)((const char*)_MyBase::m_ptr + pos), cnt);
		}
	};

	template <typename T>
	HX_INLINE ptrdiff_t operator-(const T* p1, const ArrayView<T>& p2) HX_NOEXCEPT
	{
		return p1 - p2.ptr();
	}
	HX_INLINE ptrdiff_t operator-(const void* p1, const ArrayView<void>& p2) HX_NOEXCEPT
	{
		return (char*)p1 - (char*)(p2.ptr());
	}
	template <typename T>
	HX_INLINE ptrdiff_t operator-(const ArrayView<T>& p1, const T* p2) HX_NOEXCEPT
	{
		return p1.ptr() - p2;
	}
	HX_INLINE ptrdiff_t operator-(const ArrayView<void>& p1, const void* p2) HX_NOEXCEPT
	{
		return (char*)(p1.ptr()) - (char*)p2;
	}

	template <typename T>
	HX_INLINE ptrdiff_t operator-(const T* p1, const ArrayConstView<T>& p2) HX_NOEXCEPT
	{
		return p1 - p2.ptr();
	}
	HX_INLINE ptrdiff_t operator-(const void* p1, const ArrayConstView<void>& p2) HX_NOEXCEPT
	{
		return (char*)p1 - (char*)(p2.ptr());
	}
	template <typename T>
	HX_INLINE ptrdiff_t operator-(const ArrayConstView<T>& p1, const T* p2) HX_NOEXCEPT
	{
		return p1.ptr() - p2;
	}
	HX_INLINE ptrdiff_t operator-(const ArrayConstView<void>& p1, const void* p2) HX_NOEXCEPT
	{
		return (char*)(p1.ptr()) - (char*)p2;
	}
}

template <typename T>
using HxArrayView = ::HxLibs::ArrayView<T>;
template <typename T>
using HxArrayConstView = ::HxLibs::ArrayConstView<T>;

#endif //!HX_ARRAY_VIEW
#endif //__cplusplus

#ifndef HX_ARRAY_VIEW_DEFINE

#ifdef __cplusplus
#define HX_ARRAY_VIEW_DEFINE(TYPE)\
HX_SEQUENCE_DEFINE(TYPE, TYPE)\
using TYPE##ArrayView = ::HxLibs::ArrayView<TYPE>;\
using TYPE##ArrayConstView = ::HxLibs::ArrayConstView<TYPE>;
#else //__cplusplus
#define HX_ARRAY_VIEW_DEFINE(TYPE)\
HX_SEQUENCE_DEFINE(TYPE, TYPE)\
typedef TYPE##Sequence TYPE##ArrayView;\
typedef TYPE##ConstSequence TYPE##ArrayConstView;
#endif //__cplusplus

#endif //!HX_ARRAY_VIEW_DEFINE

#ifndef HX_ARRAY_VIEW_DEFINE_ROOT_NS

#ifdef __cplusplus
#define HX_ARRAY_VIEW_DEFINE_ROOT_NS(TYPE)\
HX_SEQUENCE_DEFINE(Hx##TYPE, Hx##TYPE)\
using Hx##TYPE##ArrayView = ::HxLibs::ArrayView<Hx##TYPE>;\
using Hx##TYPE##ArrayConstView = ::HxLibs::ArrayConstView<Hx##TYPE>;\
namespace HxLibs\
{\
	HX_SEQUENCE_DEFINE(TYPE, TYPE)\
	using TYPE##ArrayView = ArrayView<TYPE>;\
	using TYPE##ArrayConstView = ArrayConstView<TYPE>;\
}
#else //__cplusplus
#define HX_ARRAY_VIEW_DEFINE_ROOT_NS(TYPE)\
HX_SEQUENCE_DEFINE(Hx##TYPE, Hx##TYPE)\
typedef Hx##TYPE##Sequence Hx##TYPE##ArrayView;\
typedef Hx##TYPE##ConstSequence Hx##TYPE##ArrayConstView;
#endif //__cplusplus

#endif //!HX_ARRAY_VIEW_DEFINE_ROOT_NS

#endif //!HX_CORE_ARRAY_VIEW_H
 