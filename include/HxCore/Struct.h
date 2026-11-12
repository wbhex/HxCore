/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STRUCT_H
#define HX_CORE_STRUCT_H

#include "Defines.h"

#ifndef HX_STRUCT_H
#define HX_STRUCT_H

#ifdef __cplusplus
#	define HX_STRUCT_DECLARE(NAME) struct NAME
#	define HX_STRUCT_DECLARE_BEGIN(NAME) struct NAME
#	define HX_STRUCT_DECLARE_END(NAME)
#	define HX_STRUCT_ALIAS(TSRC,TDST) using TDST = TSRC
#	define HX_STRUCT_FORWARD(NAME) NAME
#else //__cplusplus
#	define HX_STRUCT_DECLARE(NAME) typedef struct NAME NAME; struct NAME
#	define HX_STRUCT_DECLARE_BEGIN(NAME) struct NAME
#	define HX_STRUCT_DECLARE_END(NAME) typedef struct NAME NAME;
#	define HX_STRUCT_ALIAS(TSRC,TDST) typedef TSRC TDST
#	define HX_STRUCT_FORWARD(NAME) struct NAME
#endif //__cplusplus
#define HX_STRUCT_FORWARD_DECLARE(NAME) struct NAME

#define HX_LIBS_STRUCT_GLOBAL_NAME(NAME) Hx##NAME

#ifdef __cplusplus
#define HX_LIBS_STRUCT_DECLARE(NAME) struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME);\
namespace HxLibs\
{\
	using NAME = HX_LIBS_STRUCT_GLOBAL_NAME(NAME); \
}\
struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME)
#define HX_LIBS_STRUCT_DECLARE_BEGIN(NAME) struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME)
#define HX_LIBS_STRUCT_DECLARE_END(NAME) \
namespace HxLibs\
{\
	using NAME = HX_LIBS_STRUCT_GLOBAL_NAME(NAME); \
}
#define HX_LIBS_STRUCT_ALIAS(SRC, DST)\
using HX_LIBS_STRUCT_GLOBAL_NAME(DST) = HX_LIBS_STRUCT_GLOBAL_NAME(SRC);\
namespace HxLibs\
{\
	using DST = SRC;\
}
#define HX_LIBS_STRUCT_ALIAS_EX(SRC, DST)\
using HX_LIBS_STRUCT_GLOBAL_NAME(DST) = SRC;\
namespace HxLibs\
{\
	using DST = SRC;\
}
#else //__cplusplus
#define HX_LIBS_STRUCT_DECLARE(NAME) typedef struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME) HX_LIBS_STRUCT_GLOBAL_NAME(NAME);struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME)
#define HX_LIBS_STRUCT_DECLARE_BEGIN(NAME) struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME)
#define HX_LIBS_STRUCT_DECLARE_END(NAME) typedef struct HX_LIBS_STRUCT_GLOBAL_NAME(NAME) HX_LIBS_STRUCT_GLOBAL_NAME(NAME);
#define HX_LIBS_STRUCT_ALIAS(SRC, DST) typedef HX_LIBS_STRUCT_GLOBAL_NAME(SRC) HX_LIBS_STRUCT_GLOBAL_NAME(DST)
#define HX_LIBS_STRUCT_ALIAS_EX(SRC, DST) typedef SRC HX_LIBS_STRUCT_GLOBAL_NAME(DST)
#endif //__cplusplus

#endif //!HX_STRUCT_H

#endif //!HX_CORE_STRUCT_H
