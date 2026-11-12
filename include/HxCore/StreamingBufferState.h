/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STREAMING_BUFFER_STATE_H
#define HX_CORE_STREAMING_BUFFER_STATE_H

#ifndef HX_STREAMING_BUFFER_STATE
#define HX_STREAMING_BUFFER_STATE

#include "Struct.h"

 //表示流式缓存状态。
HX_LIBS_STRUCT_DECLARE(StreamingBufferState)
{
	//容量大小。
	size_t capacity;
	//分块大小。
	size_t blockSize;

	//有效数据大小
	size_t validSize;
	//可用空间大小
	size_t availableSize;
	//有效数据起始
	unsigned long long validFrom;
};

#endif //!HX_STREAMING_BUFFER_STATE

#endif //!HX_CORE_STREAMING_BUFFER_STATE_H
