/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CHAR_H
#define HX_CORE_CHAR_H

#include <wchar.h>

#ifndef HX_CHAR
#define HX_CHAR
typedef char HxChar;
typedef wchar_t HxWChar;
#ifdef __cplusplus
namespace HxLibs
{
	using Char = char;
	using WChar = wchar_t;  
}
#endif //__cplusplus
#endif //!HX_CHAR

#include "BufView.h"

#ifndef HX_CHARS
#define HX_CHARS

HX_BUFVIEW_DEFINE_ROOT_NS(Char)
HX_BUFVIEW_DEFINE_ROOT_NS(WChar)

#endif //!HX_CHARS

#endif //!HX_CORE_CHAR_H
