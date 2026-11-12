/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_DEFINES_H
#define HX_CORE_DEFINES_H

#include <stddef.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif //__cplusplus

#ifndef NOMINMAX
#	define NOMINMAX
#	ifdef max
#		undef max
#	endif //max
#	ifdef min
#		undef min
#	endif //min
#endif //NOMINMAX

#if !defined(HX_GCC)
#	if	defined(__GNUC__)
#		define HX_GCC 1
#	else
#		define HX_GCC 0
#	endif
#endif // HX_GCC

#if !defined(HX_MSVC)
#	if defined(_MSC_VER)
#		define HX_MSVC 1
#	else
#		define HX_MSVC 0
#	endif
#endif // HX_MSVC

#if !defined(HX_WINDOWS)
#	if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WINDOWS) || defined(_WIN32_CE) || defined(__CYGWIN__)
#		define HX_WINDOWS 1
#		define HX_DLL_INIT
#		define HX_DLL_FINI
#		define HX_API_EXPORT __declspec(dllexport)
#		define HX_LIB_EXTENSION "dll"
#		define HX_LIBNAME_PREFIX ""
#		define HX_LIBNAME_SUFFIX ".dll"
#		define HX_FILEPATH_SEPARATOR '\\'
#	else
#		define HX_WINDOWS 0
#		if HX_GCC
#			define HX_DLL_INIT __attribute__ ((constructor))
#			define HX_DLL_FINI  __attribute__ ((destructor))
#			define HX_API_EXPORT __attribute__ ((visibility("default")))
#		else
#			define HX_DLL_INIT
#			define HX_DLL_FINI
#			define HX_API_EXPORT
#		endif
#		define HX_LIB_EXTENSION "so"
#		define HX_LIBNAME_PREFIX "lib"
#		define HX_LIBNAME_SUFFIX ".so"
#		define HX_FILEPATH_SEPARATOR '/'
#	endif
#endif // HX_WINDOWS

#if !defined(HX_64BIT)
#	if HX_WINDOWS
#		if defined(_WIN64)
#			define HX_64BIT 1
#		else
#			define HX_64BIT 0
#		endif
#	elif HX_GCC
#		if defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__)
#			define HX_64BIT 1
#		else
#			define HX_64BIT 0
#		endif
#	else
#		define HX_64BIT 1
#	endif
#endif // HX_64BIT

#if !defined(HX_DEBUG)
#	if HX_MSVC
#		if defined(_DEBUG)
#			define HX_DEBUG 1
#		else
#			define HX_DEBUG 0
#		endif
#	elif HX_GCC
#		if defined(NDEBUG)
#			define HX_DEBUG 0
#		else
#			define HX_DEBUG 1
#		endif
#	else
#		if defined(_DEBUG)
#			define HX_DEBUG 1
#		else  
#			define HX_DEBUG 0
#		endif
#	endif
#endif // HX_DEBUG

#ifndef HX_INLINE
#ifdef __cplusplus
#	define HX_INLINE inline
#else //!__cplusplus
#if HX_WINDOWS
#	define inline __inline
#	define HX_INLINE __inline
#else //HX_WINDOWS
#	define HX_INLINE static inline
#endif //HX_WINDOWS
#endif //__cplusplus
#endif //!HX_INLINE

#if !defined(HX_NOINLINE)
#if HX_MSVC
#define HX_NOINLINE __declspec(noinline)
#elif HX_GCC
#define HX_NOINLINE __attribute__((noinline))
#else
#define HX_NOINLINE
#endif
#endif // HX_NOINLINE

#if !defined(HX_FORCEINLINE)
#if HX_DEBUG
#define HX_FORCEINLINE inline
#else // HX_DEBUG
#if HX_MSVC
#define HX_FORCEINLINE __forceinline
#elif HX_GCC
#define HX_FORCEINLINE inline __attribute__((always_inline))
#else
#define HX_FORCEINLINE inline
#endif
#endif // HX_DEBUG
#endif // HX_FORCEINLINE

#ifndef HX_CDECL
#if HX_WINDOWS
#	define HX_CDECL __cdecl
#else //!HX_WINDOWS
#	define HX_CDECL
#endif //HX_WINDOWS
#endif //! HX_CDECL

#ifndef HX_STDCALL
#if HX_WINDOWS
#	define HX_STDCALL __stdcall
#else //!HX_WINDOWS
#	define HX_STDCALL
#endif //HX_WINDOWS
#endif //! HX_STDCALL

#ifndef HX_NOEXCEPT
#ifndef HX_NOEXCEPT_ENABLED
#	define HX_NOEXCEPT
#else //HX_NOEXCEPT_ENABLED
#ifdef __cplusplus
#	if __cplusplus < 201103L
#		ifdef _MSVC_LANG 
#			if _MSVC_LANG < 201103L 
#				define HX_NOEXCEPT
#			else //_MSVC_LANG >= 201103L
#				define HX_NOEXCEPT noexcept
#			endif //_MSVC_LANG >= 201103L
#		else //!_MSVC_LANG
#			define HX_NOEXCEPT
#		endif //!_MSVC_LANG
#	else //__cplusplus >= 201103L
#		define HX_NOEXCEPT noexcept
#	endif //__cplusplus >= 201103L
#else //!__cplusplus
#	define HX_NOEXCEPT
#endif //__cplusplus
#endif //HX_NOEXCEPT_ENABLED
#endif //!HX_NOEXCEPT

#ifndef HX_CONSTEXPR
#ifdef __cplusplus
#	if __cplusplus < 201103L
#		ifdef _MSVC_LANG 
#			if _MSVC_LANG < 201103L
#				define HX_CONSTEXPR const
#			else //_MSVC_LANG >= 201103L
#				if _MSC_VER > 1800
#					define HX_CONSTEXPR constexpr
#				else //_MSC_VER <= 1800
#					define HX_CONSTEXPR const
#				endif //_MSC_VER <= 1800
#			endif //_MSVC_LANG >= 201103L
#		else //!_MSVC_LANG
#			define HX_CONSTEXPR const
#		endif //!_MSVC_LANG
#	else //__cplusplus >= 201103L
#		define HX_CONSTEXPR constexpr
#	endif //__cplusplus >= 201103L
#endif //__cplusplus
#endif //!HX_CONSTEXPR

#ifndef HX_CONSTEXPR11_FUNC
#ifdef __cplusplus
#	if __cplusplus < 201103L
#		ifdef _MSVC_LANG 
#			if _MSVC_LANG < 201103L
#				define HX_CONSTEXPR11_FUNC inline
#			else //_MSVC_LANG >= 201103L
#				if _MSC_VER > 1800
#					define HX_CONSTEXPR11_FUNC constexpr
#				else //_MSC_VER <= 1800
#					define HX_CONSTEXPR11_FUNC inline
#				endif //_MSC_VER <= 1800
#			endif //_MSVC_LANG >= 201103L
#		else //!_MSVC_LANG
#			define HX_CONSTEXPR11_FUNC inline
#		endif //!_MSVC_LANG
#	else //__cplusplus >= 201103L
#		define HX_CONSTEXPR11_FUNC constexpr
#	endif //__cplusplus >= 201103L
#else //!__cplusplus
#	define HX_CONSTEXPR11_FUNC inline
#endif //!__cplusplus
#endif //!HX_CONSTEXPR11_FUNC

#ifndef HX_CONSTEXPR17_FUNC
#ifdef __cplusplus
#ifdef _CONSTEXPR17
#define HX_CONSTEXPR17_FUNC _CONSTEXPR17
#else //!_CONSTEXPR17
#define HX_CONSTEXPR17_FUNC inline
#endif //!_CONSTEXPR17
#else //!__cplusplus
#	define HX_CONSTEXPR17_FUNC inline
#endif //!__cplusplus
#endif //!HX_CONSTEXPR17_FUNC

#ifndef HX_CONSTEXPR20_FUNC
#ifdef __cplusplus
#ifdef _CONSTEXPR20
#define HX_CONSTEXPR20_FUNC _CONSTEXPR20
#else //!_CONSTEXPR20
#define HX_CONSTEXPR20_FUNC inline
#endif //!_CONSTEXPR20
#else //!__cplusplus
#	define HX_CONSTEXPR20_FUNC inline
#endif //!__cplusplus
#endif //!HX_CONSTEXPR20_FUNC

#ifndef HX_SIZEOF
#define HX_SIZEOF(T) sizeof(T)
#ifdef __cplusplus
namespace HxLibs
{
	template <typename T>
	struct Sizeof
	{
		static HX_CONSTEXPR size_t value = sizeof(T);
	};
	template <>
	struct Sizeof<void>
	{
		static HX_CONSTEXPR size_t value = 1;
	};
}
template <typename T>
using HxSizeof = ::HxLibs::Sizeof<T>;
#endif //__cplusplus
#endif //!HX_SIZEOF

#ifndef HX_ALIGNOF
#ifdef __cplusplus
#	if __cplusplus < 201103L
#		ifdef _MSVC_LANG 
#			if _MSVC_LANG < 201103L
#				define HX_ALIGNOF(T) sizeof(T)
#			else //_MSVC_LANG >= 201103L
#				if _MSC_VER > 1800
#					define HX_ALIGNOF(T) alignof(T)
#				else //_MSC_VER <= 1800
#					define HX_ALIGNOF(T) sizeof(T)
#				endif //_MSC_VER <= 1800
#			endif //_MSVC_LANG >= 201103L
#		else //!_MSVC_LANG
#			define HX_ALIGNOF(T) sizeof(T)
#		endif //!_MSVC_LANG
#	else //__cplusplus >= 201103L
#		define HX_ALIGNOF(T) alignof(T)
#	endif //__cplusplus >= 201103L
namespace HxLibs
{
	template <typename T>
	struct Alignof
	{
		static HX_CONSTEXPR size_t value = HX_ALIGNOF(T);
	};
	template <>
	struct Alignof<void>
	{
		static HX_CONSTEXPR size_t value = HX_ALIGNOF(char);
	};
}
template <typename T>
using HxAlignof = ::HxLibs::Alignof<T>;
#endif //__cplusplus
#endif //!HX_ALIGNOF

#ifndef HX_DEPRECATED
#	if (defined( __ICL ) || defined( __ECL ) || defined(_MSC_VER)) && !defined( _PCS ) && !defined( _PCS_GENSTUBS )
#		if( __INTEL_COMPILER >= 1100 ) /* icl 11.0 supports additional comment */
#			if( _MSC_VER >= 1400 )
#				define HX_DEPRECATED( comment ) __declspec( deprecated ( comment ))
#			else
#				pragma message ("your icl version supports additional comment for deprecated functions but it can't be displayed")
#				pragma message ("because internal _MSC_VER macro variable setting requires compatibility with MSVC7.1")
#				pragma message ("use -Qvc8 switch for icl command line to see these additional comments")
#				define HX_DEPRECATED( comment ) __declspec( deprecated )
#			endif
#		elif( _MSC_FULL_VER >= 140050727 )&&( !defined( __INTEL_COMPILER )) /* VS2005 supports additional comment */
#			define HX_DEPRECATED( comment ) __declspec( deprecated ( comment ))
#		elif( _MSC_VER <= 1200 )&&( !defined( __INTEL_COMPILER )) /* VS 6 doesn't support deprecation */
#			define HX_DEPRECATED( comment )
#		else
#			define HX_DEPRECATED( comment ) __declspec( deprecated )
#		endif
#	elif (defined(__ICC) || defined(__ECC) || defined( __GNUC__ )) && !defined( _PCS ) && !defined( _PCS_GENSTUBS )
#		if defined( __GNUC__ )
#			if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#				define HX_DEPRECATED( message ) __attribute__(( deprecated( message )))
#			else
#				define HX_DEPRECATED( message ) __attribute__(( deprecated ))
#			endif
#		else
#			define HX_DEPRECATED( comment ) __attribute__(( deprecated ))
#		endif
#	else
#			define HX_DEPRECATED( comment )
#	endif
#endif //!HX_DEPRECATED

#ifndef HX_STATIC_ASSERT
#ifdef __cplusplus
#	if __cplusplus < 201103L
#		ifdef _MSVC_LANG 
#			if _MSVC_LANG < 201103L
#				define HX_STATIC_ASSERT(EXPR, MSGTXT)
#			else //_MSVC_LANG >= 201103L
#				if _MSC_VER > 1800
#					define HX_STATIC_ASSERT(EXPR, MSGTXT) static_assert(EXPR, MSGTXT)
#				else //_MSC_VER <= 1800
#					define HX_STATIC_ASSERT(EXPR, MSGTXT)
#				endif //_MSC_VER <= 1800
#			endif //_MSVC_LANG >= 201103L
#		else //!_MSVC_LANG
#			define HX_STATIC_ASSERT(EXPR, MSGTXT)
#		endif //!_MSVC_LANG
#	else //__cplusplus >= 201103L
#		define HX_STATIC_ASSERT(EXPR, MSGTXT) static_assert(EXPR, MSGTXT)
#	endif //__cplusplus >= 201103L
#else //!__cplusplus
#	define HX_STATIC_ASSERT(EXPR, MSGTXT)
#endif //!__cplusplus
#endif //!HX_STATIC_ASSERT

#ifndef HX_CHECK_POWEROF2
#define HX_CHECK_POWEROF2(N) (N > 0 && (N & (N - 1)) == 0)
#ifdef __cplusplus
namespace HxLibs
{
	HX_CONSTEXPR11_FUNC static bool checkPowerOf2(const size_t n) HX_NOEXCEPT
	{//判断是否为2的阶。
		return n > 0 && (n & (n - 1)) == 0;
	}
}
#endif //__cplusplus
#endif //!HX_CHECK_POWEROF2

#endif //!HX_CORE_DEFINES_H
