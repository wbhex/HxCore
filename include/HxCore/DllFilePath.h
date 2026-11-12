/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_DLL_FILE_PATH_H
#define HX_CORE_DLL_FILE_PATH_H

#ifdef __cplusplus
#ifndef HX_DLL_FILE_PATH
#define HX_DLL_FILE_PATH

#ifdef HX_DLL_FILE_PATH_IMPL
#if HX_WINDOWS
#include <Windows.h>
#else //!HX_WINDOWS
#include <unistd.h>
#include <dlfcn.h>
#endif //!HX_WINDOWS
#endif //HX_DLL_FILE_PATH_IMPL
#include "FilePathEntry.h"

namespace HxLibs
{
	class DllFilePath
	{
	private:
		//当前值指针
		static FilePathEntry* _current;

	public:
#ifdef HX_DLL_FILE_PATH_IMPL
		//执行初始化处理。
		HX_INLINE static void init(::std::filesystem::path&& path) HX_NOEXCEPT
		{
			_current = new FilePathEntry(::std::move(path));
		}
#if HX_WINDOWS
		//执行Windows平台初始化处理。
		HX_INLINE static void init(HMODULE hmodule) HX_NOEXCEPT
		{
			char fileName[0x400];
			GetModuleFileNameA(hmodule, fileName, sizeof(fileName));
			::std::filesystem::path currentLibPath(fileName);
			init(::std::move(currentLibPath.parent_path().lexically_normal()));
		}
#else //!HX_WINDOWS
		//执行Linux平台初始化处理。
		HX_INLINE static void init(void* libFunc) HX_NOEXCEPT
		{
			bool ok = false;
			Dl_info dlInfo;
			int res = dladdr(libFunc, &dlInfo);
			if (res != 0)
			{
				::std::filesystem::path dlInfoPath(dlInfo.dli_fname);
				::std::error_code errorCode;
				dlInfoPath = ::std::filesystem::canonical(dlInfoPath, errorCode);
				if (!errorCode)
				{
					init(::std::move(dlInfoPath.parent_path().lexically_normal()));
					ok = true;
				}
			}
			if (!ok)
			{
				char fileName[0x400] = { 0 };
				readlink("/proc/self/exe", fileName, sizeof(fileName));
				::std::filesystem::path applicationPath(fileName);
				init(::std::move(applicationPath.parent_path().lexically_normal()));
			}
		}
#endif //!HX_WINDOWS
		//执行释放处理。
		HX_INLINE static void free() HX_NOEXCEPT
		{
			delete _current;
			_current = nullptr;
		}
#endif //HX_DLL_FILE_PATH_IMPL

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
#ifdef HX_DLL_FILE_PATH_IMPL
	FilePathEntry* DllFilePath::_current;
#endif //HX_DLL_FILE_PATH_IMPL
}
using HxDllFilePath = ::HxLibs::DllFilePath;

#endif //HX_DLL_FILE_PATH
#endif //__cplusplus

#endif //!HX_CORE_DLL_FILE_PATH_H
