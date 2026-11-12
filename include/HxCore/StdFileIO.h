/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_STD_FILEIO_H
#define HX_STD_FILEIO_H

#ifdef __cplusplus
#include <cstdio>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/stat.h>

#include "FileIOShareMode.h"
#include "FileIOAccessMode.h"
#include "FileIOSeekOrigin.h"
#include "FileIOCreationMode.h"
#include "Uncopyable.h"
#include "String.h"

#if HX_WINDOWS
#include <io.h>
#else //!HX_WINDOWS
#include <unistd.h>
#endif //!HX_WINDOWS

namespace HxLibs
{
	class StdFileIO : Uncopyable
	{
	private:
		static void makeOpenFlags(FileIOCreationMode creation, FileIOAccessMode access, FileIOShareMode share, int& flags, String& mode, int& permission) HX_NOEXCEPT
		{
			assert(((int)access >= 1 && (int)access <= 3));
			assert(((int)creation >= 1 && (int)creation <= 6));
			assert(!((creation == FileIOCreationMode::CREATE_OR_APPEND) && access == FileIOAccessMode::READ));

			if (creation == FileIOCreationMode::CREATE_OR_APPEND)
			{
				flags = O_CREAT | O_APPEND;
				if (access == FileIOAccessMode::WRITE)
				{
					flags |= O_WRONLY;
					mode.append("ab");
					permission = S_IWRITE;
				}
				else if (access == FileIOAccessMode::READ_WRITE)
				{
					flags |= O_RDWR;
					mode.append("ab+");
					permission = S_IREAD | S_IWRITE;
				}
			}
			else
			{
				if (access == FileIOAccessMode::READ)
				{
					flags = O_RDONLY;
					mode.append("rb");
					permission = S_IREAD;
				}
				else if (access == FileIOAccessMode::WRITE)
				{
					flags = O_WRONLY;
					mode.append("wb");
					permission = S_IWRITE;
				}
				else if (access == FileIOAccessMode::READ_WRITE)
				{
					flags = O_RDWR;
					mode.append("wb+");
					permission = S_IREAD | S_IWRITE;
				}

				if (creation == FileIOCreationMode::CREATE)
				{
					flags |= O_CREAT | O_EXCL;
				}
				else if (creation == FileIOCreationMode::CREATE_OR_TRUNCATE)
				{
					flags |= O_CREAT | O_TRUNC;
				}
				else if (creation == FileIOCreationMode::OPEN_OR_CREATE)
				{
					flags |= O_CREAT;
				}
				else if (creation == FileIOCreationMode::TRUNCATE)
				{
					flags |= O_TRUNC;
				}
			}
#if HX_WINDOWS
			flags |= O_BINARY;
#endif //HX_WINDOWS
		}

	protected:
		FILE* m_fp{ nullptr };

	public:
		HX_INLINE FILE* handle() const HX_NOEXCEPT
		{
			return m_fp;
		}

		HX_INLINE bool close() HX_NOEXCEPT
		{
			if (m_fp)
			{
				fclose(m_fp);
				m_fp = nullptr;
			}
			return true;
		}

		HX_INLINE bool open(const char* path, FileIOCreationMode creation, FileIOAccessMode access, FileIOShareMode share)
		{
			close();

			int flags;
			String mode;
			int permission;
			makeOpenFlags(creation, access, share, flags, mode, permission);

#if HX_WINDOWS
			int fd = _open(path, flags, permission);
#else //!HX_WINDOWS
			int fd = ::open(path, flags, permission);
#endif //!HX_WINDOWS
			if (fd != -1)
			{
				m_fp = fdopen(fd, mode.c_str());
				if (m_fp != nullptr)
				{
					return true;
				}
#if HX_WINDOWS
				_close(fd);
#else //!HX_WINDOWS
				::close(fd);
#endif //!HX_WINDOWS
			}
			return false;
		}
#if HX_WINDOWS
		HX_INLINE bool open(const wchar_t* path, FileIOCreationMode creation, FileIOAccessMode access, FileIOShareMode share)
		{
			close();

			int flags;
			String mode;
			int permission;
			makeOpenFlags(creation, access, share, flags, mode, permission);

			int fd = _wopen(path, flags, permission);
			if (fd != -1)
			{
				m_fp = fdopen(fd, mode.c_str());
				if (m_fp != nullptr)
				{
					return true;
				}
				_close(fd);
			}
			return false;
		}
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

		HX_INLINE bool isOpen() const HX_NOEXCEPT
		{
			return m_fp != nullptr;
		}
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return isOpen();
		}

		HX_INLINE size_t read(void* buffer, size_t elmSize, size_t elmCount)
		{
			assert(m_fp);
			return fread(buffer, elmSize, elmCount, m_fp);
		}
		HX_INLINE size_t read(void* buffer, size_t elmCount)
		{
			return read(buffer, 1, elmCount);
		}

		HX_INLINE size_t write(const void* buffer, size_t elmSize, size_t elmCount)
		{
			assert(m_fp);
			return fwrite(buffer, elmSize, elmCount, m_fp);
		}
		HX_INLINE size_t write(const void* buffer, size_t elmCount)
		{
			return write(buffer, 1, elmCount);
		}

		HX_INLINE bool flush()
		{
			assert(m_fp);
			return fflush(m_fp) == 0;
		}

		HX_INLINE long long tell()
		{
			assert(m_fp);
#if HX_WINDOWS
			return _ftelli64(m_fp);
#else //!HX_WINDOWS
			return ftello64(m_fp);
#endif //!HX_WINDOWS
		}

		HX_INLINE long long seek(long long offset, FileIOSeekOrigin origin)
		{
			assert(m_fp);
#if HX_WINDOWS
			switch (origin)
			{
			case FileIOSeekOrigin::CUR:
				return _fseeki64(m_fp, offset, SEEK_CUR);
			case FileIOSeekOrigin::END:
				return _fseeki64(m_fp, offset, SEEK_END);
			case FileIOSeekOrigin::SET:
			default:
				return _fseeki64(m_fp, offset, SEEK_SET);
			}
#else //!HX_WINDOWS
			switch (origin)
			{
			case FileIOSeekOrigin::CUR:
				return fseeko64(m_fp, offset, SEEK_CUR);
			case FileIOSeekOrigin::END:
				return fseeko64(m_fp, offset, SEEK_END);
			case FileIOSeekOrigin::SET:
			default:
				return fseeko64(m_fp, offset, SEEK_SET);
			}
#endif //!HX_WINDOWS
		}
		HX_INLINE bool rewind()
		{
			return seek(0, FileIOSeekOrigin::SET) == 0;
		}

		HX_INLINE size_t printf(const char* fmt, va_list args)
		{
			assert(m_fp);
			return vfprintf(m_fp, fmt, args);
		}
		HX_INLINE size_t printf(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			size_t ret = printf(fmt, args);
			va_end(args);
			return ret;
		}

		HX_INLINE long long getFileSize()
		{
			assert(m_fp);
#if HX_WINDOWS
			auto curPos = _ftelli64(m_fp);
			if (_fseeki64(m_fp, 0, SEEK_END) != 0)
				return -1;
			auto size = _ftelli64(m_fp);
			_fseeki64(m_fp, curPos, SEEK_SET);
#else //!HX_WINDOWS
			auto curPos = ftello64(m_fp);
			if (fseeko64(m_fp, 0, SEEK_END) != 0)
				return -1;
			auto size = ftello64(m_fp);
			fseeko64(m_fp, curPos, SEEK_SET);
#endif //!HX_WINDOWS
			return size;
		}

		HX_INLINE bool setBufferSize(int bufferSize)
		{
			if (m_fp)
			{
				return 0 == setvbuf(m_fp, NULL, _IOFBF, bufferSize);
			}
			return false;
		}

		HX_INLINE StdFileIO() HX_NOEXCEPT
		{
			//NOTHING
		}

		HX_INLINE ~StdFileIO()
		{
			close();
		}
	};
}
using HxStdFileIO = HxLibs::StdFileIO;

#endif //__cplusplus

#endif //!HX_STD_FILEIO_H
