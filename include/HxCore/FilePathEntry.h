/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_FILE_PATH_ENTRY_H
#define HX_CORE_FILE_PATH_ENTRY_H

#ifdef __cplusplus
#ifndef HX_FILE_PATH_ENTRY
#define HX_FILE_PATH_ENTRY

#include <filesystem>
#include "String.h"
namespace HxLibs
{
	class FilePathEntry
	{
	private:
		::std::filesystem::path m_value;
		String m_string;

	public:
		HX_INLINE FilePathEntry(const ::std::filesystem::path& value) HX_NOEXCEPT
			: m_value(value)
			, m_string(m_value.template string<char, String::traits_type, String::allocator_type>())
		{
			//NOTHING
		}
		HX_INLINE FilePathEntry(::std::filesystem::path&& value) HX_NOEXCEPT
			: m_value(::std::move(value))
			, m_string(m_value.template string<char, String::traits_type, String::allocator_type>())
		{
			//NOTHING
		}

		HX_INLINE operator const ::std::filesystem::path&() const HX_NOEXCEPT
		{
			return m_value;
		}
		HX_INLINE const ::std::filesystem::path& value() const HX_NOEXCEPT
		{
			return m_value;
		}

		HX_INLINE operator const String&() const HX_NOEXCEPT
		{
			return m_string;
		}
		HX_INLINE const String& string() const HX_NOEXCEPT
		{
			return m_string;
		}
	};
}
using HxFilePathEntry = ::HxLibs::FilePathEntry;
#endif //HX_FILE_PATH_ENTRY
#endif //__cplusplus

#endif //!HX_CORE_FILE_PATH_ENTRY_H
