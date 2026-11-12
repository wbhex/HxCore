/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_EXE_FILE_PATH_H
#define HX_CORE_EXE_FILE_PATH_H

#ifdef __cplusplus
#ifndef HX_EXE_FILE_PATH
#define HX_EXE_FILE_PATH

#ifdef HX_EXE_FILE_PATH_IMPL
#if HX_WINDOWS
#include <Windows.h>
#else //!HX_WINDOWS
#include <unistd.h>
#include <dlfcn.h>
#endif //!HX_WINDOWS
#endif //HX_EXE_FILE_PATH_IMPL
#include "FilePathEntry.h"

namespace HxLibs
{
	class ExeFilePath
	{
	private:
		//当前值指针
		static FilePathEntry* _current;

	public:
#ifdef HX_EXE_FILE_PATH_IMPL
		//执行初始化处理。
		HX_INLINE static void init(::std::filesystem::path&& path) HX_NOEXCEPT
		{
			_current = new FilePathEntry(::std::move(path));
		}
#if HX_WINDOWS
		//执行Windows平台初始化处理。
		HX_INLINE static void init() HX_NOEXCEPT
		{
			char fileName[0x400];
			GetModuleFileNameA(NULL, fileName, sizeof(fileName));
			::std::filesystem::path path(fileName);
			init(::std::move(path.parent_path().lexically_normal()));
		}
#else //!HX_WINDOWS
		//执行Linux平台初始化处理。
		HX_INLINE static void init() HX_NOEXCEPT
		{
			char fileName[0x400] = { 0 };
			readlink("/proc/self/exe", fileName, sizeof(fileName));
			::std::filesystem::path path(fileName);
			init(::std::move(path.parent_path().lexically_normal()));
		}
#endif //!HX_WINDOWS
		//执行释放处理。
		HX_INLINE static void free() HX_NOEXCEPT
		{
			delete _current;
			_current = nullptr;
		}
#endif //HX_EXE_FILE_PATH_IMPL

		//当前值
		HX_INLINE static const FilePathEntry& current() HX_NOEXCEPT
		{
			return *_current;
		}

		//当前路径值
		HX_INLINE static const ::std::filesystem::path& currentValue() HX_NOEXCEPT
		{
			return _current->value();
		}
		//当前路径字符串
		HX_INLINE static const String& currentString() HX_NOEXCEPT
		{
			return _current->string();
		}
	};
#ifdef HX_EXE_FILE_PATH_IMPL
	FilePathEntry* ExeFilePath::_current;
#endif //HX_EXE_FILE_PATH_IMPL
}
using HxExeFilePath = ::HxLibs::ExeFilePath;

#endif //HX_EXE_FILE_PATH
#endif //__cplusplus

#endif //!HX_CORE_EXE_FILE_PATH_H
