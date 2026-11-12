/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_BYTE_H
#define HX_CORE_BYTE_H

#ifndef HX_BYTE
#define HX_BYTE
typedef unsigned char HxByte;
#ifdef __cplusplus
namespace HxLibs
{
	using Byte = unsigned char;
}
#endif //__cplusplus
#endif //!HX_BYTE

#include "BufView.h"

#ifndef HX_BYTES
#define HX_BYTES

HX_BUFVIEW_DEFINE_ROOT_NS(Byte)

#endif //!HX_BYTES

#endif //!HX_CORE_BYTE_H
