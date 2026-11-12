/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_ACCESS_MODE_H
#define HX_FILEIO_ACCESS_MODE_H

#ifdef __cplusplus
#include "Enum.h"

namespace HxLibs
{
	enum class FileIOAccessMode
	{
		READ = 1,		// 读取
		WRITE = 2,		// 写入
		READ_WRITE = 3	// 读写
	};
	using FileAccessMode = FileIOAccessMode;
	HX_FLAG_ENUM_MAKE(FileIOAccessMode);
}
using HxFileAccessMode = HxLibs::FileIOAccessMode;
using HxFileIOAccessMode = HxLibs::FileIOAccessMode;
#endif //__cplusplus

#endif //!HX_FILEIO_ACCESS_MODE_H
