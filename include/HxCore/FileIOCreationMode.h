/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_FILEIO_CREATION_MODE_H
#define HX_FILEIO_CREATION_MODE_H

#ifdef __cplusplus
namespace HxLibs
{
	enum class FileIOCreationMode
	{
		CREATE = 1,				// 创建新文件，如果文件存在将失败
		CREATE_OR_TRUNCATE = 2,	// 创建新文件，如果文件存在将覆盖
		OPEN = 3,				// 打开已经存在的文件，如果文件不存在将失败
		OPEN_OR_CREATE = 4,		// 打开或创建一个文件
		TRUNCATE = 5,			// 打开一个已经存在的文件并删除所有数据，如果文件不存在将失败
		CREATE_OR_APPEND = 6,	// 打开或创建一个文件并定位到文件末尾以便后续进行数据追加
	};
	using FileCreationMode = FileIOCreationMode;
}
using HxFileCreationMode = HxLibs::FileIOCreationMode;
using HxFileIOCreationMode = HxLibs::FileIOCreationMode;
#endif //__cplusplus

#endif //!HX_FILEIO_CREATION_MODE_H
