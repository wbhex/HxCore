/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_SEEK_ORIGIN_H
#define HX_FILEIO_SEEK_ORIGIN_H

#ifdef __cplusplus
namespace HxLibs
{
	enum class FileIOSeekOrigin
	{
		SET = 0,	// 起始位置
		BEGIN = 0,	// 起始位置
		CUR = 1,	// 当前位置
		CURRENT = 1,// 当前位置
		END = 2		// 结束位置
	};
	using FileSeekOrigin = FileIOSeekOrigin;
}
using HxFileSeekOrigin = HxLibs::FileIOSeekOrigin;
using HxFileIOSeekOrigin = HxLibs::FileIOSeekOrigin;
#endif //__cplusplus

#endif //!HX_FILEIO_SEEK_ORIGIN_H
