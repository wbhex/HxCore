/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_BUFVIEW_H
#define HX_CORE_BUFVIEW_H

#include "ArrayView.h"

#ifndef HX_BUFVIEW
#define HX_BUFVIEW

#ifdef __cplusplus
namespace HxLibs
{
	template <typename T>
	using BufView = ArrayView<T>;
	template <typename T>
	using BufConstView = ArrayConstView<T>;

	using RawBufView = BufView<void>;
	using RawBufConstView = BufConstView<void>;
}
using HxRawBufView = ::HxLibs::BufView<void>;
using HxRawBufConstView = ::HxLibs::BufConstView<void>;
#else //__cplusplus
typedef HxRawSequence HxRawBufView;
typedef HxRawConstSequence HxRawBufConstView;
#endif //__cplusplus

#endif //!HX_BUFVIEW

#ifndef HX_BUFVIEW_DEFINE

#ifdef __cplusplus
#define HX_BUFVIEW_DEFINE(TYPE)\
HX_ARRAY_VIEW_DEFINE(TYPE)\
using TYPE##BufView = ::HxLibs::BufView<TYPE>;\
using TYPE##BufConstView = ::HxLibs::BufConstView<TYPE>;
#else //__cplusplus
#define HX_BUFVIEW_DEFINE(TYPE)\
HX_ARRAY_VIEW_DEFINE(TYPE)\
typedef TYPE##Sequence TYPE##BufView;\
typedef TYPE##ConstSequence TYPE##BufConstView;
#endif //__cplusplus

#endif //!HX_BUFVIEW_DEFINE

#ifndef HX_BUFVIEW_DEFINE_ROOT_NS

#ifdef __cplusplus
#define HX_BUFVIEW_DEFINE_ROOT_NS(TYPE)\
HX_ARRAY_VIEW_DEFINE_ROOT_NS(TYPE)\
using Hx##TYPE##BufView = ::HxLibs::BufView<Hx##TYPE>;\
using Hx##TYPE##BufConstView = ::HxLibs::BufConstView<Hx##TYPE>;\
namespace HxLibs\
{\
	using TYPE##BufView = BufView<TYPE>;\
	using TYPE##BufConstView = BufConstView<TYPE>;\
}
#else //__cplusplus
#define HX_BUFVIEW_DEFINE_ROOT_NS(TYPE)\
HX_ARRAY_VIEW_DEFINE(Hx##TYPE)\
typedef Hx##TYPE##Sequence Hx##TYPE##BufView;\
typedef Hx##TYPE##ConstSequence Hx##TYPE##BufConstView;
#endif //__cplusplus

#endif //!HX_BUFVIEW_DEFINE_ROOT_NS

#endif //!HX_CORE_BUFVIEW_H
