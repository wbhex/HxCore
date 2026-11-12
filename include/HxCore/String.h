/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRING_H
#define HX_CORE_STRING_H

#ifdef __cplusplus

#ifndef HX_CHAR_DEFAULT_TRAITS
#define HX_CHAR_DEFAULT_TRAITS

#ifndef HX_CHAR_DEFAULT_TRAITS_CUSTOM
#	ifdef HX_STRING_CSTRING_NULL_SUPPORT
#		define HX_CHAR_DEFAULT_TRAITS_CUSTOM
#	endif //HX_STRING_CUSTOM
#endif //!HX_CHAR_DEFAULT_TRAITS_CUSTOM

#ifdef HX_CHAR_DEFAULT_TRAITS_CUSTOM
#include "HashUtils.h"
#include "CStringLength.h"
#define HX_CHAR_DEFAULT_CUSTOM_TRAITS
namespace HxLibs
{
	template <typename T>
	struct CharDefaultTraits : public ::std::char_traits<T>
	{
		HX_CONSTEXPR17_FUNC static size_t length(const T* const str) HX_NOEXCEPT
		{
			return CStringLength::count<T>(str);
		}
	};
}
namespace std
{
	template <typename T, typename Alloc>
	struct hash<basic_string<T, ::HxLibs::CharDefaultTraits<T>, Alloc>>
	{
		using result_type = size_t;
		using argument_type = basic_string<T, ::HxLibs::CharDefaultTraits<T>, Alloc>;

		HX_INLINE size_t operator()(const basic_string<T, ::HxLibs::CharDefaultTraits<T>, Alloc>& v) const HX_NOEXCEPT
		{
			if (v.size() > 0)
			{
				return ::HxHash_fnv1a((const unsigned char*)(v.c_str()), v.size());
			}
			return 0;
		}
	};
}
#else //!HX_CHAR_DEFAULT_TRAITS_CUSTOM
#include <string>
#define HX_CHAR_DEFAULT_STD_TRAITS
namespace HxLibs
{
	template <typename T>
	using CharDefaultTraits = ::std::char_traits<T>;
}
#endif //!HX_CHAR_DEFAULT_TRAITS_CUSTOM

template <typename T>
using HxCharDefaultTraits = ::HxLibs::CharDefaultTraits<T>;

#endif //!HX_CHAR_DEFAULT_TRAITS

#ifndef HX_STRING_DEFAULT_ALLOCATOR
#define HX_STRING_DEFAULT_ALLOCATOR

#ifndef HX_STRING_DEFAULT_ALLOCATOR_CUSTOM
#	ifdef HX_STRING_CUSTOM
#		define HX_STRING_DEFAULT_ALLOCATOR_CUSTOM
#	endif //HX_STRING_CUSTOM
#endif //!HX_STRING_DEFAULT_ALLOCATOR_CUSTOM

#ifndef HX_STRING_DEFAULT_ALLOCATOR_STDPMR
#	ifdef HX_STRING_STDPMR
#		define HX_STRING_DEFAULT_ALLOCATOR_STDPMR
#	endif //HX_STRING_STDPMR
#endif //!HX_STRING_DEFAULT_ALLOCATOR_STDPMR

#ifdef HX_STRING_DEFAULT_ALLOCATOR_CUSTOM
#include "MemoryResourceAllocator.h"
#define HX_STRING_DEFAULT_MEMORY_RESOURCE_ALLOCATOR
namespace HxLibs
{
	template <typename T>
	using StringDefaultAllocator = MemoryResourceAllocator<T>;
}
#else //!HX_STRING_DEFAULT_ALLOCATOR_CUSTOM
#	if _HAS_CXX17 && defined HX_STRING_DEFAULT_ALLOCATOR_STDPMR
#include <memory_resource>
#define HX_STRING_DEFAULT_STDPMR_ALLOCATOR
namespace HxLibs
{
	template <typename T>
	using StringDefaultAllocator = ::std::pmr::polymorphic_allocator<T>;
}
#	else //!HX_STRING_DEFAULT_ALLOCATOR_STDPMR
#include <memory>
#define HX_STRING_DEFAULT_STD_ALLOCATOR
namespace HxLibs
{
	template <typename T>
	using StringDefaultAllocator = ::std::allocator<T>;
}
#	endif //!HX_STRING_DEFAULT_ALLOCATOR_STDPMR
#endif //!HX_STRING_DEFAULT_ALLOCATOR_CUSTOM

template <typename T>
using HxStringDefaultAllocator = ::HxLibs::StringDefaultAllocator<T>;

#endif //!HX_STRING_DEFAULT_ALLOCATOR

#ifndef HX_STRING
#define HX_STRING

#include <string>
#include <cstring>

namespace HxLibs
{
	template <typename Char = char, typename Traits = CharDefaultTraits<Char>, typename Alloc = StringDefaultAllocator<Char>>
	using XString = ::std::basic_string<Char, Traits, Alloc>;

	using String = XString<char, CharDefaultTraits<char>, StringDefaultAllocator<char>>;
	using WString = XString<wchar_t, CharDefaultTraits<wchar_t>, StringDefaultAllocator<wchar_t>>;
}

template <typename Char = char, typename Traits = HxCharDefaultTraits<Char>, typename Alloc = HxStringDefaultAllocator<Char>>
using HxXString = ::HxLibs::XString<Char, Traits, Alloc>;

using HxString = ::HxLibs::String;
using HxWString = ::HxLibs::WString;

#endif //!HX_STRING
#endif //__cplusplus

#endif //!HX_CORE_STRING_H
