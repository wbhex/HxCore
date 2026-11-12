/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SEQUENCE_H
#define HX_CORE_SEQUENCE_H

#ifndef HX_SEQUENCE
#define HX_SEQUENCE

#ifdef __cplusplus

#include <type_traits>
#include "Defines.h"

namespace HxLibs
{
	template <typename T>
	class Sequence
	{
		HX_STATIC_ASSERT((::std::is_pointer<T>::value), "::HxLibs::Sequence<T> Type parameter must be pointer.");

	public:
		T ptr;
		size_t size;

		HX_CONSTEXPR11_FUNC Sequence() HX_NOEXCEPT
			: ptr(nullptr), size(0)
		{
			//NOTHING
		}
		HX_CONSTEXPR11_FUNC Sequence(T ptr, size_t size) HX_NOEXCEPT
			: ptr(ptr), size(size)
		{
			//NOTHING
		}

		HX_INLINE T operator()() const HX_NOEXCEPT
		{
			return ptr;
		}

		HX_INLINE operator T () const HX_NOEXCEPT
		{
			return ptr;
		}
	};
}

#define HX_SEQUENCE_DEFINE(NAME, TYPE)\
using NAME##Sequence = ::HxLibs::Sequence<TYPE*>;\
using NAME##ConstSequence = ::HxLibs::Sequence<const TYPE*>;

#else //__cplusplus

#include <stddef.h>

#define HX_SEQUENCE_DEFINE(NAME, TYPE) \
typedef struct NAME##Sequence \
{ \
	TYPE* ptr; \
	size_t size; \
} NAME##Sequence;\
typedef struct NAME##ConstSequence \
{\
	const TYPE* ptr; \
	size_t size; \
} NAME##ConstSequence;

#endif //__cplusplus

HX_SEQUENCE_DEFINE(HxRaw, void); 
#ifdef __cplusplus
namespace HxLibs
{
	HX_SEQUENCE_DEFINE(Raw, void);
}
#endif //__cplusplus

#endif //!HX_SEQUENCE

#endif //!HX_CORE_SEQUENCE_H
