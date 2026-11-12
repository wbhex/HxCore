/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_H
#define HX_FILEIO_H

#ifdef __cplusplus

#if HX_WINDOWS
#include "WinFileIO.h"
namespace HxLibs
{
	using DefaultFileIO = WinFileIO;
}
#else //!HX_WINDOWS
#include "StdFileIO.h"
namespace HxLibs
{
	using DefaultFileIO = StdFileIO;
}
#endif //HX_WINDOWS
using HxDefaultFileIO = HxLibs::DefaultFileIO;

#include "FileIOAbstract.h"

namespace HxLibs
{
	template <typename IO = DefaultFileIO>
	class FileIO : public FileIOAbstract, public IO
	{
	public:
		virtual bool close() override
		{
			return IO::close();
		}

		virtual bool open(const char* path, FileIOCreationMode creation, FileIOAccessMode access, FileIOShareMode share) override
		{
			return IO::open(path, creation, access, share);
		}
#if HX_WINDOWS
		virtual bool open(const wchar_t* path, FileIOCreationMode creation, FileIOAccessMode access, FileIOShareMode share) override
		{
			return IO::open(path, creation, access, share);
		}
#endif //HX_WINDOWS

		HX_INLINE bool openRead(const char* path)
		{
			return IO::open(path, FileIOCreationMode::OPEN, FileIOAccessMode::READ, FileIOShareMode::READ_WRITE);
		}
		HX_INLINE bool openWrite(const char* path)
		{
			return IO::open(path, FileIOCreationMode::CREATE_OR_TRUNCATE, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
		HX_INLINE bool openAppend(const char* path)
		{
			return IO::open(path, FileIOCreationMode::CREATE_OR_APPEND, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}

#if HX_WINDOWS
		HX_INLINE bool openRead(const wchar_t* path)
		{
			return IO::open(path, FileIOCreationMode::OPEN, FileIOAccessMode::READ, FileIOShareMode::READ_WRITE);
		}
		HX_INLINE bool openWrite(const wchar_t* path)
		{
			return IO::open(path, FileIOCreationMode::CREATE_OR_TRUNCATE, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
		HX_INLINE bool openAppend(const wchar_t* path)
		{
			return IO::open(path, FileIOCreationMode::CREATE_OR_APPEND, FileIOAccessMode::READ_WRITE, FileIOShareMode::READ);
		}
#endif //HX_WINDOWS

		virtual bool isOpen() const HX_NOEXCEPT override
		{
			return IO::isOpen();
		}
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return IO::isOpen();
		}

		virtual size_t read(void* buffer, size_t elmSize, size_t elmCount) override
		{
			return IO::read(buffer, elmSize, elmCount);
		}
		HX_INLINE size_t read(void* buffer, size_t elmCount)
		{
			return IO::read(buffer, 1, elmCount);
		}

		virtual size_t write(const void* buffer, size_t elmSize, size_t elmCount) override
		{
			return IO::write(buffer, elmSize, elmCount);
		}
		HX_INLINE size_t write(const void* buffer, size_t elmCount)
		{
			return IO::write(buffer, 1, elmCount);
		}

		virtual bool flush() override
		{
			return IO::flush();
		}

		virtual long long tell() override
		{
			return IO::tell();
		}

		virtual long long seek(long long offset, FileIOSeekOrigin origin) override
		{
			return IO::seek(offset, origin);
		}
		HX_INLINE bool rewind()
		{
			return IO::seek(0, FileIOSeekOrigin::SET) == 0;
		}

		virtual size_t printf(const char* fmt, va_list args) override
		{
			return IO::printf(fmt, args);
		}
		HX_INLINE size_t printf(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			size_t ret = IO::printf(fmt, args);
			va_end(args);
			return ret;
		}

		virtual long long getFileSize() override
		{
			return IO::getFileSize();
		}
		virtual bool setBufferSize(int bufferSize) override
		{
			return IO::setBufferSize(bufferSize);
		}
	};
}
template <typename IO = HxDefaultFileIO>
using HxFileIO = HxLibs::FileIO<IO>;

#endif //__cplusplus

#endif //!HX_FILEIO_H
