/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_STRING_UUID_STRING_H
#define HX_STRING_UUID_STRING_H

#ifndef HX_UUID_STRING
#define HX_UUID_STRING

#include "Uuid.h"

 //将UUID格式化为字符串。
HX_INLINE int HxUuid_toStringA(const HxUuid* v, char* buffer, size_t bufSize) HX_NOEXCEPT
{
	return sprintf(buffer, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		v->data1,
		(unsigned short)v->data2,
		v->data3,
		v->data4[0], v->data4[1],
		v->data4[2], v->data4[3], v->data4[4], v->data4[5], v->data4[6], v->data4[7]);
}
//将UUID格式化为字符串。
HX_INLINE int HxUuid_toStringW(const HxUuid* v, wchar_t* buffer, size_t bufSize) HX_NOEXCEPT
{
	return swprintf(buffer, bufSize, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		v->data1,
		(unsigned short)v->data2,
		v->data3,
		v->data4[0], v->data4[1],
		v->data4[2], v->data4[3], v->data4[4], v->data4[5], v->data4[6], v->data4[7]);
}

//从字符串从创建一个UUID。
HX_INLINE bool HxUuid_fromStringA(HxUuid* v, const char* text) HX_NOEXCEPT
{
#if HX_WINDOWS && defined(HX_UUID_WINRPC)
	return UuidFromStringA((RPC_CSTR)text, (UUID*)v) == RPC_S_OK;
#else //!HX_WINDOWS && HX_UUID_WINRPC
	int n = sscanf(text, "%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&v->data1,
		&v->data2,
		&v->data3,
		&v->data4[0], &v->data4[1],
		&v->data4[2], &v->data4[3], &v->data4[4], &v->data4[5], &v->data4[6], &v->data4[7]);
	return n == 11;
#endif //!HX_WINDOWS && HX_UUID_WINRPC
}
//从字符串从创建一个UUID。
HX_INLINE bool HxUuid_fromStringW(HxUuid* v, const wchar_t* text) HX_NOEXCEPT
{
#if HX_WINDOWS && defined(HX_UUID_WINRPC)
	return UuidFromStringW((RPC_WSTR)text, (UUID*)v) == RPC_S_OK;
#else //!HX_WINDOWS && HX_UUID_WINRPC
	int n = swscanf(text, L"%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
		&v->data1,
		&v->data2,
		&v->data3,
		&v->data4[0], &v->data4[1],
		&v->data4[2], &v->data4[3], &v->data4[4], &v->data4[5], &v->data4[6], &v->data4[7]);
	return n == 11;
#endif //!HX_WINDOWS && HX_UUID_WINRPC
}

#ifdef __cplusplus
#define HX_UUID_STRBUF_SIZE 64
#include "String.h"
#include "CString.h"

//转换为字符串
HX_INLINE int HxUuid_toString(const HxUuid* v, char* buffer, size_t bufSize) HX_NOEXCEPT
{
	return HxUuid_toStringA(v, buffer, bufSize);
}
//转换为字符串
HX_INLINE int HxUuid_toString(const HxUuid* v, wchar_t* buffer, size_t bufSize) HX_NOEXCEPT
{
	return HxUuid_toStringW(v, buffer, bufSize);
}
//转换为字符串
HX_INLINE int HxUuid_toString(const HxUuid& v, char* buffer, size_t bufSize) HX_NOEXCEPT
{
	return HxUuid_toStringA(&v, buffer, bufSize);
}
//转换为字符串
HX_INLINE int HxUuid_toString(const HxUuid& v, wchar_t* buffer, size_t bufSize) HX_NOEXCEPT
{
	return HxUuid_toStringW(&v, buffer, bufSize);
}

//转换为字符串
template <typename T, typename Traits, typename Alloc>
HX_INLINE void HxUuid_toString(const HxUuid& uuid, HxXString<T, Traits, Alloc>& text) HX_NOEXCEPT
{
	T buffer[HX_UUID_STRBUF_SIZE] = { 0 };
	auto length = HxUuid_toString(uuid, buffer, HX_UUID_STRBUF_SIZE);
	text.assign(buffer, length);
}

//转换为字符串
template <typename T = char, typename Traits = HxCharDefaultTraits<T>, typename Alloc = HxStringDefaultAllocator<T>>
HX_INLINE HxXString<T, Traits, Alloc> HxUuid_toString(const HxUuid& uuid, const Alloc& alloc = Alloc{}) HX_NOEXCEPT
{
	T buffer[HX_UUID_STRBUF_SIZE] = { 0 };
	auto length = HxUuid_toString(uuid, buffer, HX_UUID_STRBUF_SIZE);
	return HxXString<T, Traits, Alloc>(buffer, length, alloc);
}

//转换为字符串
template <typename Traits = HxCharDefaultTraits<char>, typename Alloc = HxStringDefaultAllocator<char>>
HX_INLINE HxXString<char, Traits, Alloc> HxUuid_toStringA(const HxUuid& uuid, const Alloc& alloc = Alloc{}) HX_NOEXCEPT
{
	return HxUuid_toString<char, Traits, Alloc>(uuid, alloc);
}
//转换为字符串
template <typename Traits = HxCharDefaultTraits<wchar_t>, typename Alloc = HxStringDefaultAllocator<wchar_t>>
HX_INLINE HxXString<wchar_t, Traits, Alloc> HxUuid_toStringW(const HxUuid& uuid, const Alloc& alloc = Alloc{}) HX_NOEXCEPT
{
	return HxUuid_toString<wchar_t, Traits, Alloc>(uuid, alloc);
}

//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(const char* text, HxUuid* uuid) HX_NOEXCEPT
{
	return HxUuid_fromStringA(uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(const wchar_t* text, HxUuid* uuid) HX_NOEXCEPT
{
	return HxUuid_fromStringW(uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(const char* text, HxUuid& uuid) HX_NOEXCEPT
{
	return HxUuid_fromStringA(&uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(const wchar_t* text, HxUuid& uuid) HX_NOEXCEPT
{
	return HxUuid_fromStringW(&uuid, text);
}

//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(HxUuid* uuid, const char* text) HX_NOEXCEPT
{
	return HxUuid_fromStringA(uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(HxUuid* uuid, const wchar_t* text) HX_NOEXCEPT
{
	return HxUuid_fromStringW(uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(HxUuid& uuid, const char* text) HX_NOEXCEPT
{
	return HxUuid_fromStringA(&uuid, text);
}
//从字符串创建对象。
HX_INLINE bool HxUuid_fromString(HxUuid& uuid, const wchar_t* text) HX_NOEXCEPT
{
	return HxUuid_fromStringW(&uuid, text);
}

//从字符串创建对象。
template <typename T, typename Traits, typename Alloc>
HX_INLINE bool HxUuid_fromString(const HxXString<T, Traits, Alloc>& text, HxUuid& uuid) HX_NOEXCEPT
{
	return HxUuid_fromString(text.c_str(), uuid);
}
//从字符串创建对象。
template <typename T, typename Traits, typename Alloc>
HX_INLINE bool HxUuid_fromString(HxUuid& uuid, const HxXString<T, Traits, Alloc>& text) HX_NOEXCEPT
{
	return HxUuid_fromString(text.c_str(), uuid);
}

//从字符串创建对象。
template <typename T>
HX_INLINE bool HxUuid_fromString(const HxXCString<T>& text, HxUuid& uuid) HX_NOEXCEPT
{
	return HxUuid_fromString(text.c_str(), uuid);
}
//从字符串创建对象。
template <typename T>
HX_INLINE bool HxUuid_fromString(HxUuid& uuid, const HxXCString<T>& text) HX_NOEXCEPT
{
	return HxUuid_fromString(text.c_str(), uuid);
}

//从字符串创建对象。
HX_INLINE HxUuid HxUuid_fromString(const char* text) HX_NOEXCEPT
{
	HxUuid uuid;
	if (!HxUuid_fromString(text, uuid))
	{
		uuid.firstData = 0LL;
		uuid.secondData = 0LL;
	}
	return uuid;
}
//从字符串创建对象。
HX_INLINE HxUuid HxUuid_fromString(const wchar_t* text) HX_NOEXCEPT
{
	HxUuid uuid;
	if (!HxUuid_fromString(text, uuid))
	{
		uuid.firstData = 0LL;
		uuid.secondData = 0LL;
	}
	return uuid;
}
//从字符串创建对象。
template <typename T, typename Traits, typename Alloc>
HX_INLINE HxUuid HxUuid_fromString(const HxXString<T, Traits, Alloc>& text) HX_NOEXCEPT
{
	HxUuid uuid;
	if (!HxUuid_fromString(text, uuid))
	{
		uuid.firstData = 0LL;
		uuid.secondData = 0LL;
	}
	return uuid;
}
//从字符串创建对象。
template <typename T>
HX_INLINE HxUuid HxUuid_fromString(const HxXCString<T>& text) HX_NOEXCEPT
{
	HxUuid uuid;
	if (!HxUuid_fromString(text, uuid))
	{
		uuid.firstData = 0LL;
		uuid.secondData = 0LL;
	}
	return uuid;
}

namespace HxLibs
{
	//转换为字符串
	template <typename T = char, typename Traits = CharDefaultTraits<T>, typename Alloc = StringDefaultAllocator<T>>
	HX_INLINE XString<T, Traits, Alloc> toString(const Uuid& uuid, const Alloc alloc = Alloc{}) HX_NOEXCEPT
	{
		return HxUuid_toString<T, Traits, Alloc>(uuid, alloc);
	}

	//从字符串创建对象。
	HX_INLINE bool fromString(const char* text, Uuid& uuid) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
	//从字符串创建对象。
	HX_INLINE bool fromString(const wchar_t* text, Uuid& uuid) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
	//从字符串创建对象。
	HX_INLINE bool fromString(Uuid& uuid, const char* text) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
	//从字符串创建对象。
	HX_INLINE bool fromString(Uuid& uuid, const wchar_t* text) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}

	//从字符串创建对象。
	template <typename T, typename Traits, typename Alloc>
	HX_INLINE bool fromString(const XString<T, Traits, Alloc>& text, Uuid& uuid) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
	//从字符串创建对象。
	template <typename T, typename Traits, typename Alloc>
	HX_INLINE bool fromString(Uuid& uuid, const XString<T, Traits, Alloc>& text) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}

	//从字符串创建对象。
	template <typename T>
	HX_INLINE bool fromString(const XCString<T>& text, Uuid& uuid) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
	//从字符串创建对象。
	template <typename T>
	HX_INLINE bool fromString(Uuid& uuid, const XCString<T>& text) HX_NOEXCEPT
	{
		return HxUuid_fromString(text, uuid);
	}
}
#else //!__cplusplus
#ifdef UNICODE
#define HxUuid_toString HxUuid_toStringW
#define HxUuid_fromString HxUuid_fromStringW
#else //!UNICODE
#define HxUuid_toString HxUuid_toStringA
#define HxUuid_fromString HxUuid_fromStringA
#endif //UNICODE
#endif //!__cplusplus

#endif //!HX_UUID_STRING

#endif //!HX_STRING_UUID_STRING_H
