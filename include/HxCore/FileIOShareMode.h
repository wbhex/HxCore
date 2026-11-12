/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_SHARE_MODE_H
#define HX_FILEIO_SHARE_MODE_H

#ifdef __cplusplus
#include "Enum.h"

namespace HxLibs
{
	enum class FileIOShareMode
	{
		NONE = 0,
		READ = 1,		// 读取
		WRITE = 2,		// 写入
		READ_WRITE = 3	// 读写
	};
	using FileShareMode = FileIOShareMode;
	HX_FLAG_ENUM_MAKE(FileIOShareMode);
}
using HxFileShareMode = HxLibs::FileIOShareMode;
using HxFileIOShareMode = HxLibs::FileIOShareMode;
#endif //__cplusplus

#endif //!HX_FILEIO_SHARE_MODE_H
