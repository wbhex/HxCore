/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_WIN_FILEIO_H
#define HX_WIN_FILEIO_H

#ifdef __cplusplus
#include <stdarg.h>
#include <assert.h>

#include "FileIOShareMode.h"
#include "FileIOAccessMode.h"
#include "FileIOSeekOrigin.h"
#include "FileIOCreationMode.h"

#if HX_WINDOWS

#ifndef HX_WIN_FILEIO_PRINTF_DEFAULT_BUFSIZE
#define HX_WIN_FILEIO_PRINTF_DEFAULT_BUFSIZE 512
#endif //!HX_WIN_FILEIO_PRINTF_DEFAULT_BUFSIZE

#include <cstdio>
#include <Windows.h>

namespace HxLibs
{
	class WinFileIO
	{
	private:
		HX_INLINE static void makeOpenFlags(FileIOCreationMode creation1, FileIOAccessMode access1, FileIOShareMode share1, DWORD& creation2, DWORD& access2, DWORD& share2) HX_NOEXCEPT
		{
			assert(((int)access1 >= 1 && (int)access1 <= 3));
			assert(((int)creation1 >= 1 && (int)creation1 <= 6));
			assert(!((creation1 == FileIOCreationMode::CREATE_OR_APPEND) && access1 == FileIOAccessMode::READ));

			creation2 = (DWORD)creation1;
			if (creation1 == FileIOCreationMode::CREATE_OR_APPEND)
			{
				creation2 = OPEN_ALWAYS;
			}

			access2 = 0;
			if ((access1 & FileIOAccessMode::READ) == FileIOAccessMode::READ)
			{
				access2 |= GENERIC_READ;
			}
			if ((access1 & FileIOAccessMode::WRITE) == FileIOAccessMode::WRITE)
			{
				access2 |= GENERIC_WRITE;
			}

			share2 = (DWORD)share1;
		}

	protected:
		HANDLE m_hFile{ INVALID_HANDLE_VALUE };

		WinFileIO(const WinFileIO&) = delete;
		WinFileIO& operator=(const WinFileIO&) = delete;

	public:
		HX_INLINE HANDLE handle() const HX_NOEXCEPT
		{
			return m_hFile;
		}

		HX_INLINE bool close() HX_NOEXCEPT
		{
			bool ret = false;
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				ret = CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
			return ret;
		}

		HX_INLINE bool open(const char* path, FileIOCreationMode creation1, FileIOAccessMode access1, FileIOShareMode share1)
		{
			close();

			DWORD creation2, access2, share2;
			makeOpenFlags(creation1, access1, share1, creation2, access2, share2);

			m_hFile = CreateFileA(path, access2, share2, NULL, creation2, 0, NULL);
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				if (creation1 == FileIOCreationMode::CREATE_OR_APPEND)
				{
					seek(0, FileIOSeekOrigin::END);
				}
				return true;
			}
			return false;
		}
		HX_INLINE bool open(const wchar_t* path, FileIOCreationMode creation1, FileIOAccessMode access1, FileIOShareMode share1)
		{
			close();

			DWORD creation2, access2, share2;
			makeOpenFlags(creation1, access1, share1, creation2, access2, share2);

			m_hFile = CreateFileW(path, access2, share2, NULL, creation2, 0, NULL);
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				if (creation1 == FileIOCreationMode::CREATE_OR_APPEND)
				{
					seek(0, FileIOSeekOrigin::END);
				}
				return true;
			}
			return false;
		}

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

		HX_INLINE bool isOpen() const HX_NOEXCEPT
		{
			return m_hFile != INVALID_HANDLE_VALUE;
		}
		HX_INLINE operator bool() const HX_NOEXCEPT
		{
			return isOpen();
		}

		HX_INLINE size_t read(void* buffer, size_t elmSize, size_t elmCount)
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			const size_t READ_MAX_LEN = 1 << 31;
			size_t leftLen = elmSize * elmCount;
			size_t readLen = 0;
			while (leftLen > 0)
			{
				auto curLen = leftLen > READ_MAX_LEN ? (DWORD)READ_MAX_LEN : (DWORD)leftLen;
				if (!ReadFile(m_hFile, (char*)buffer + readLen, curLen, &curLen, NULL) || curLen == 0)
				{
					break;
				}
				leftLen -= curLen;
				readLen += curLen;
			}
			return readLen / elmSize;
		}
		HX_INLINE size_t read(void* buffer, size_t elmCount)
		{
			return read(buffer, 1, elmCount);
		}

		HX_INLINE size_t write(const void* buffer, size_t elmSize, size_t elmCount)
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			const size_t WRITE_MAX_LEN = 1 << 31;
			size_t leftLen = elmSize * elmCount;
			size_t writtenLen = 0;
			while (leftLen > 0)
			{
				auto curLen = leftLen > WRITE_MAX_LEN ? (DWORD)WRITE_MAX_LEN : (DWORD)leftLen;
				if (!WriteFile(m_hFile, (char*)buffer + writtenLen, curLen, &curLen, NULL) || curLen == 0)
				{
					break;
				}
				leftLen -= curLen;
				writtenLen += curLen;
			}
			return writtenLen / elmSize;
		}
		HX_INLINE size_t write(const void* buffer, size_t elmCount)
		{
			return write(buffer, 1, elmCount);
		}

		HX_INLINE bool flush() HX_NOEXCEPT
		{
			return m_hFile != INVALID_HANDLE_VALUE;
		}

		HX_INLINE long long tell()
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			LARGE_INTEGER liPos = { 0 };
			liPos.LowPart = ::SetFilePointer(m_hFile, NULL, &liPos.HighPart, FILE_CURRENT);
			return liPos.QuadPart;
		}

		HX_INLINE long long seek(long long offset, FileIOSeekOrigin origin)
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			LARGE_INTEGER liPos = { 0 };
			liPos.QuadPart = offset;
			assert(m_hFile);
			switch (origin)
			{
			case FileIOSeekOrigin::CUR:
				liPos.LowPart = ::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart, FILE_CURRENT);
				break;
			case FileIOSeekOrigin::END:
				liPos.LowPart = ::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart, FILE_END);
				break;
			case FileIOSeekOrigin::SET:
			default:
				liPos.LowPart = ::SetFilePointer(m_hFile, liPos.LowPart, &liPos.HighPart, FILE_BEGIN);
				break;
			}
			return liPos.QuadPart;
		}
		HX_INLINE bool rewind()
		{
			return seek(0, FileIOSeekOrigin::SET) == 0;
		}

		template <size_t BUFSIZE = HX_WIN_FILEIO_PRINTF_DEFAULT_BUFSIZE>
		HX_INLINE size_t printf(const char* fmt, va_list args)
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			char buffer[BUFSIZE];
			int size = _vsnprintf(buffer, BUFSIZE, fmt, args);
			if (size > 0)
			{
				return write(buffer, 1, (size_t)size);
			}
			return 0;
		}
		template <size_t BUFSIZE = HX_WIN_FILEIO_PRINTF_DEFAULT_BUFSIZE>
		HX_INLINE size_t printf(const char* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			size_t ret = printf<BUFSIZE>(fmt, args);
			va_end(args);
			return ret;
		}

		HX_INLINE long long getFileSize()
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			LARGE_INTEGER fileSize = { 0 };
			if (GetFileSizeEx(m_hFile, &fileSize))
			{
				return fileSize.QuadPart;
			}
			return 0;
		}

		HX_INLINE bool setBufferSize(int bufferSize) HX_NOEXCEPT
		{
			return false;
		}

		HX_INLINE WinFileIO() HX_NOEXCEPT
			: m_hFile(INVALID_HANDLE_VALUE)
		{
			//NOTHING
		}
		HX_INLINE ~WinFileIO()
		{
			close();
		}
	};
}
using HxWinFileIO = HxLibs::WinFileIO;

#endif //HX_WINDOWS
#endif //__cplusplus

#endif //!HX_WIN_FILEIO_H
