/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_WIN_DIRECT_FILEIO_H
#define HX_WIN_DIRECT_FILEIO_H

#ifdef __cplusplus
#if HX_WINDOWS
#include <cstdio>
#include <assert.h>
#include <Windows.h>

#include "WinFileIO.h"
#include "SimpleBuffer.h"
#include "StringView.h"
#include "String.h"

namespace HxLibs
{
	class WinDirectFileIO : public WinFileIO
	{
	private:
		template <typename Char>
		HX_INLINE static XStringView<Char> getRootPath(const XStringView<Char>& path)
		{
			if (path.size() > 0)
			{
				auto idx = path.size() - 1;
				while (idx > 0)
				{
					auto& ch = path[idx];
					if (ch != '/' && ch != '\\')
					{
						--idx;
					}
					else
					{
						return XStringView<Char>(path.ptr(), idx);
					}
				}
				return XStringView<Char>();
			}
			return path;
		}

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
		//使用FILE_FLAG_NO_BUFFERING读取或写入的文件必须按照扇区对齐写入。
		SimpleBuffer<char> m_writeAlignBuffer;
		int m_writeAlignBufferDataLen{ 0 };
		int m_writeAlignBufferLen{ 0 };

	public:
		HX_INLINE bool close()
		{
			bool ret = false;
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				flush();
				m_writeAlignBufferDataLen = 0;
				m_writeAlignBufferLen = 0;
				m_writeAlignBuffer.reset();
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

			DWORD dwFlagsAndAttributes = 0;
			if ((access1 & FileIOAccessMode::WRITE) == FileIOAccessMode::WRITE)
			{
				dwFlagsAndAttributes = FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
				if (!m_writeAlignBuffer)
				{
					auto rootPathView = getRootPath(StringView(path));
					char rootPath[1024];
					memcpy(rootPath, rootPathView.ptr(), rootPathView.size());
					rootPath[rootPathView.size()] = '\0';

					DWORD lpSectorsPerCluster;
					DWORD lpBytesPerSector;
					DWORD lpNumberOfFreeClusters;
					DWORD lpTotalNumberOfClusters;
					if (!GetDiskFreeSpaceA(rootPath, &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters))
					{
						return false;
					}
					m_writeAlignBufferLen = lpBytesPerSector;
					m_writeAlignBuffer.realloc(m_writeAlignBufferLen);
				}
				m_writeAlignBufferDataLen = 0;
			}

			m_hFile = CreateFileA(path, access2, share2, NULL, creation2, dwFlagsAndAttributes, NULL);
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

			DWORD dwFlagsAndAttributes = 0;
			if ((access1 & FileIOAccessMode::WRITE) == FileIOAccessMode::WRITE)
			{
				dwFlagsAndAttributes = FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH;
				if (!m_writeAlignBuffer)
				{
					auto rootPathView = getRootPath(WStringView(path));
					wchar_t rootPath[1024];
					memcpy(rootPath, rootPathView.ptr(), rootPathView.size() * sizeof(wchar_t));
					rootPath[rootPathView.size()] = '\0';

					DWORD lpSectorsPerCluster;
					DWORD lpBytesPerSector;
					DWORD lpNumberOfFreeClusters;
					DWORD lpTotalNumberOfClusters;
					if (!GetDiskFreeSpaceW(rootPath, &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters))
					{
						return false;
					}
					m_writeAlignBufferLen = lpBytesPerSector;
					m_writeAlignBuffer.realloc(m_writeAlignBufferLen);
				}
				m_writeAlignBufferDataLen = 0;
			}

			m_hFile = CreateFileW(path, access2, share2, NULL, creation2, dwFlagsAndAttributes, NULL);
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
			const size_t WRITE_ALIGN_MAX_LEN = (WRITE_MAX_LEN / m_writeAlignBufferLen) * m_writeAlignBufferLen;
			size_t leftLen = elmSize * elmCount;
			size_t writtenLen = 0;
			if (m_writeAlignBufferDataLen > 0)
			{
				auto bufLen = m_writeAlignBufferLen - m_writeAlignBufferDataLen;
				if (bufLen == 0)
				{
					DWORD curLen = 0;
					if (!WriteFile(m_hFile, m_writeAlignBuffer.ptr(), (DWORD)m_writeAlignBufferLen, &curLen, NULL) || curLen == 0)
					{
						return 0;
					}
					m_writeAlignBufferDataLen = 0;
				}
				else if (bufLen <= leftLen)
				{
					memcpy(m_writeAlignBuffer.ptr() + m_writeAlignBufferDataLen, buffer, bufLen);
					m_writeAlignBufferDataLen += (int)bufLen;
					writtenLen += bufLen;
					leftLen -= bufLen;
					DWORD curLen = 0;
					if (!WriteFile(m_hFile, m_writeAlignBuffer.ptr(), (DWORD)m_writeAlignBufferLen, &curLen, NULL) || curLen == 0)
					{
						return writtenLen / elmSize;
					}
					m_writeAlignBufferDataLen = 0;
				}
			}
			while (leftLen >= m_writeAlignBufferLen)
			{
				auto alignLen = (leftLen / m_writeAlignBufferLen) * m_writeAlignBufferLen;
				auto curLen = alignLen > WRITE_ALIGN_MAX_LEN ? (DWORD)WRITE_ALIGN_MAX_LEN : (DWORD)alignLen;
				if (!WriteFile(m_hFile, (char*)buffer + writtenLen, curLen, &curLen, NULL) || curLen == 0)
				{
					return writtenLen / elmSize;
				}
				leftLen -= curLen;
				writtenLen += curLen;
			}
			if (leftLen > 0)
			{
				memcpy(m_writeAlignBuffer.ptr() + m_writeAlignBufferDataLen, (char*)buffer + writtenLen, leftLen);
				m_writeAlignBufferDataLen += (int)leftLen;
				writtenLen += leftLen;
			}
			return writtenLen / elmSize;
		}
		HX_INLINE size_t write(const void* buffer, size_t elmCount)
		{
			return write(buffer, 1, elmCount);
		}

		HX_INLINE bool flush()
		{
			assert(m_hFile != INVALID_HANDLE_VALUE);
			if (m_writeAlignBuffer && m_writeAlignBufferDataLen > 0)
			{
				memset(m_writeAlignBuffer.ptr() + m_writeAlignBufferDataLen, 0, m_writeAlignBufferLen - m_writeAlignBufferDataLen);
				DWORD curWriteLen = m_writeAlignBufferLen;
				auto ret = WriteFile(m_hFile, m_writeAlignBuffer.ptr(), (DWORD)m_writeAlignBufferLen, &curWriteLen, NULL) != 0;
				m_writeAlignBufferDataLen = 0;
				return ret;
			}
			return true;
		}

		HX_INLINE WinDirectFileIO() HX_NOEXCEPT
		{
			//NOTHING
		}
		HX_INLINE ~WinDirectFileIO()
		{
			close();
		}
	};
}
using HxWinDirectFileIO = HxLibs::WinDirectFileIO;

#endif //HX_WINDOWS
#endif //__cplusplus

#endif //!HX_WIN_DIRECT_FILEIO_H
