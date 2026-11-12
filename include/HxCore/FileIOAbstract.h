/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_ABSTRACT_H
#define HX_FILEIO_ABSTRACT_H

#ifdef __cplusplus
#include <stdarg.h>
#include "FileIOShareMode.h"
#include "FileIOAccessMode.h"
#include "FileIOSeekOrigin.h"
#include "FileIOCreationMode.h"

namespace HxLibs
{
#if HX_WINDOWS
	struct __declspec(novtable) FileIOAbstract
#else //!HX_WINDOWS
	struct FileIOAbstract
#endif //!HX_WINDOWS
	{
		virtual bool close() = 0;

		virtual bool open(const char* path, FileIOCreationMode, FileIOAccessMode, FileIOShareMode) = 0;
#if HX_WINDOWS
		virtual bool open(const wchar_t* path, FileIOCreationMode, FileIOAccessMode, FileIOShareMode) = 0;
#endif //HX_WINDOWS

		HX_INLINE bool openRead(const char* path)
		{
			return open(path, FileIOCreationMode::OPEN, FileIOAccessMode::READ, FileIOShareMode::READ_WRITE);
		}
		HX_INLINE bool openWrite(const char* path)
		{
			return open(path, FileIOCreationMode::CREATE_OR_TRUNCATE, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
		HX_INLINE bool openAppend(const char* path)
		{
			return open(path, FileIOCreationMode::CREATE_OR_APPEND, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}

#if HX_WINDOWS
		HX_INLINE bool openRead(const wchar_t* path)
		{
			return open(path, FileIOCreationMode::OPEN, FileIOAccessMode::READ, FileIOShareMode::READ_WRITE);
		}
		HX_INLINE bool openWrite(const wchar_t* path)
		{
			return open(path, FileIOCreationMode::CREATE_OR_TRUNCATE, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
		HX_INLINE bool openAppend(const wchar_t* path)
		{
			return open(path, FileIOCreationMode::CREATE_OR_APPEND, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
#endif //HX_WINDOWS

		virtual bool isOpen() const HX_NOEXCEPT = 0;
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return isOpen();
		}

		virtual size_t read(void* buffer, size_t elmSize, size_t elmCount) = 0;
		HX_INLINE size_t read(void* buffer, size_t elmCount)
		{
			return read(buffer, 1, elmCount);
		}

		virtual size_t write(const void* buffer, size_t elmSize, size_t elmCount) = 0;
		HX_INLINE size_t write(const void* buffer, size_t elmCount)
		{
			return write(buffer, 1, elmCount);
		}

		virtual bool flush() = 0;

		virtual long long tell() = 0;

		virtual long long seek(long long offset, FileIOSeekOrigin origin) = 0;
		HX_INLINE bool rewind()
		{
			return seek(0, FileIOSeekOrigin::SET) == 0;
		}

		virtual size_t printf(const char* fmt, va_list args) = 0;
		HX_INLINE size_t printf(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			size_t ret = printf(fmt, args);
			va_end(args);
			return ret;
		}

		virtual long long getFileSize() = 0;

		virtual bool setBufferSize(int bufferSize) = 0;
	};
}
using HxFileIOAbstract = HxLibs::FileIOAbstract;
#endif //__cplusplus

#endif //!HX_FILEIO_ABSTRACT_H
