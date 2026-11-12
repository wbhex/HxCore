/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STREAMING_BUFFER_SEGMENT_H
#define HX_CORE_STREAMING_BUFFER_SEGMENT_H

#ifndef HX_STREAMING_BUFFER_SEGMENT
#define HX_STREAMING_BUFFER_SEGMENT

#include "BufView.h"
#include "IntTypes.h"
#include "Referencable.h"

#ifdef __cplusplus
namespace HxLibs
{
	template <typename T>
	HX_INTERFACE_DECLARE(StreamingBufferSegment) : public Referencable
	{
		//获取缓存视图对象并返回缓存大小：
		//	如果缓存区间在缓存池尾部和首部，view2将为首部区间视图，否则为空；
		//	如果view2参数为空，则将仅返回尾部区间视图；
		//	如果from为空，则将返回区间起始。
		HX_METHOD(size_t, retrieve)(BufConstView<T> *view1, BufConstView<T> *view2, Hx64u* from) HX_CONST HX_NOEXCEPT = 0;

		//锁定额外的指定区间的缓存视图以进行读取并返回实际的区间起始，如果指定区间不能锁定将会缩小锁定的区间范围：
		//	如果缓存区间在缓存池尾部和首部，view2将为首部区间视图，否则为空;
		//	如果view2参数为空，则锁定的大小不会包含首部区间视图。
		HX_METHOD(Hx64u, lockExtraView)(Hx64u from, size_t size, BufConstView<T> *view1, BufConstView<T> *view2) HX_NOEXCEPT = 0;
		//尝试锁定额外的指定区间的缓存视图以进行读取，如果指定的区间不满足读取条件将返回0，否则返回区间大小。
		//	如果缓存区间在缓存池尾部和首部，view2将为首部区间视图，否则为空。
		//	如果view2参数为空且锁定的区间包含首部区间时将不会锁定该首部区间，返回大小只包含缓存池尾部区间。
		HX_METHOD(size_t, tryLockExtraView)(Hx64u from, size_t size, BufConstView<T> *view1, BufConstView<T> *view2) HX_NOEXCEPT = 0;
		//释放之前被锁定的额外缓存视图。
		HX_METHOD(void, unlockExtraView)(BufConstView<T> view) HX_NOEXCEPT = 0;

		//锁定额外的指定区间的缓存片段以进行读取，如果指定区间不能锁定将会缩小锁定的区间范围。
		HX_METHOD(StreamingBufferSegment*, lockExtraSegment)(Hx64u from, size_t size, BufConstView<T> *view1, BufConstView<T> *view2, Hx64u* fromActual) HX_NOEXCEPT = 0;
		//尝试锁定额外的指定区间的缓存片段以进行读取，如果指定的区间不满足读取条件将返回NULL。
		HX_METHOD(StreamingBufferSegment*, tryLockExtraSegment)(Hx64u from, size_t size, BufConstView<T> *view1, BufConstView<T> *view2) HX_NOEXCEPT = 0;

		//获取缓存视图对象。
		HX_INLINE BufConstView<T> retrieve() const HX_NOEXCEPT
		{
			BufConstView<T> view;
			retrieve(&view, nullptr, nullptr);
			return view;
		}
		//获取缓存视图对象。
		HX_INLINE BufConstView<T> retrieve(Hx64u& from) const HX_NOEXCEPT
		{
			BufConstView<T> view;
			retrieve(&view, nullptr, &from);
			return view;
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> *view) const HX_NOEXCEPT
		{
			return retrieve(view, nullptr, nullptr);
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> &view) const HX_NOEXCEPT
		{
			return retrieve(&view, nullptr, nullptr);
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> &view, Hx64u& from) const HX_NOEXCEPT
		{
			return retrieve(&view, nullptr, &from);
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> *view1, BufConstView<T> *view2) const HX_NOEXCEPT
		{
			return retrieve(view1, view2, nullptr);
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> &view1, BufConstView<T> &view2) const HX_NOEXCEPT
		{
			return retrieve(&view1, &view2, nullptr);
		}
		//获取缓存视图对象。
		HX_INLINE size_t retrieve(BufConstView<T> &view1, BufConstView<T> &view2, Hx64u& from) const HX_NOEXCEPT
		{
			return retrieve(&view1, &view2, &from);
		}

		//锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE BufConstView<T> lockExtraView(Hx64u from, size_t size) HX_NOEXCEPT
		{
			BufConstView<T> view;
			lockExtraView(from, size, &view, nullptr);
			return view;
		}
		//锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE BufConstView<T> lockExtraView(Hx64u from, size_t size, Hx64u& fromActual) HX_NOEXCEPT
		{
			BufConstView<T> view;
			fromActual = lockExtraView(from, size, &view, nullptr);
			return view;
		}
		//锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE Hx64u lockExtraView(Hx64u from, size_t size, BufConstView<T> *view) HX_NOEXCEPT
		{
			return lockExtraView(from, size, view, nullptr);
		}
		//锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE Hx64u lockExtraView(Hx64u from, size_t size, BufConstView<T> &view) HX_NOEXCEPT
		{
			return lockExtraView(from, size, &view, nullptr);
		}
		//锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE Hx64u lockExtraView(Hx64u from, size_t size, BufConstView<T> &view1, BufConstView<T> &view2) HX_NOEXCEPT
		{
			return lockExtraView(from, size, &view1, &view2);
		}

		//尝试锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE BufConstView<T> tryLockExtraView(Hx64u from, size_t size) HX_NOEXCEPT
		{
			BufConstView<T> view;
			tryLockExtraView(from, size, &view, nullptr);
			return view;
		}
		//尝试锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE size_t tryLockExtraView(Hx64u from, size_t size, BufConstView<T> *view) HX_NOEXCEPT
		{
			return tryLockExtraView(from, size, view, nullptr);
		}
		//尝试锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE size_t tryLockExtraView(Hx64u from, size_t size, BufConstView<T> &view) HX_NOEXCEPT
		{
			return tryLockExtraView(from, size, &view, nullptr);
		}
		//尝试锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE size_t tryLockExtraView(Hx64u from, size_t size, BufConstView<T>&view1, BufConstView<T> *view2) HX_NOEXCEPT
		{
			return tryLockExtraView(from, size, &view1, view2);
		}
		//尝试锁定额外的指定区间的缓存视图以进行读取。
		HX_INLINE size_t tryLockExtraView(Hx64u from, size_t size, BufConstView<T> &view1, BufConstView<T> &view2) HX_NOEXCEPT
		{
			return tryLockExtraView(from, size, &view1, &view2);
		}

		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, nullptr, nullptr);
		}
		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size, BufConstView<T> *view) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, view, nullptr, nullptr);
		}
		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size, BufConstView<T> &view) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, &view, nullptr, nullptr);
		}
		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size, BufConstView<T> *view1, BufConstView<T> *view2) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, view1, view2, nullptr);
		}
		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size, BufConstView<T> &view1, BufConstView<T> &view2) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, &view1, &view2, nullptr);
		}
		//锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* lockExtraSegment(Hx64u from, size_t size, BufConstView<T> &view1, BufConstView<T> &view2, Hx64u& fromActual) HX_NOEXCEPT
		{
			return lockExtraSegment(from, size, &view1, &view2, &fromActual);
		}

		//尝试锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* tryLockExtraSegment(Hx64u from, size_t size) HX_NOEXCEPT
		{
			return tryLockExtraSegment(from, size, nullptr, nullptr);
		}
		//尝试锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* tryLockExtraSegment(Hx64u from, size_t size, BufConstView<T> *view) HX_NOEXCEPT
		{
			return tryLockExtraSegment(from, size, view, nullptr);
		}
		//尝试锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* tryLockExtraSegment(Hx64u from, size_t size, BufConstView<T> &view) HX_NOEXCEPT
		{
			return tryLockExtraSegment(from, size, &view, nullptr);
		}
		//尝试锁定额外的指定区间的缓存片段以进行读取。
		HX_INLINE StreamingBufferSegment* tryLockExtraSegment(Hx64u from, size_t size, BufConstView<T> &view1, BufConstView<T> &view2) HX_NOEXCEPT
		{
			return tryLockExtraSegment(from, size, &view1, &view2);
		}
	};
	using RawStreamingBufferSegment = StreamingBufferSegment<void>;
}
using HxRawStreamingBufferSegment = ::HxLibs::StreamingBufferSegment<void>;
#else //!__cplusplus
#define HX_STREAMING_BUFFER_SEGMENT_DECLARE(NAME, BUFVIEW)\
HX_REFERENCABLE_DECLARE_BEGIN(NAME)\
{\
	HX_METHOD(size_t, retrieve)(HX_CONST_THIS_(NAME) BUFVIEW * view1, BUFVIEW * view2, Hx64u* from) HX_CONST HX_NOEXCEPT HX_PURE;\
	HX_METHOD(Hx64u, lockExtraView)(HX_THIS_(NAME) Hx64u from, size_t size, BUFVIEW * view1, BUFVIEW * view2) HX_NOEXCEPT HX_PURE;\
	HX_METHOD(size_t, tryLockExtraView)(HX_THIS_(NAME) Hx64u from, size_t size, BUFVIEW * view1, BUFVIEW * view2) HX_NOEXCEPT HX_PURE;\
	HX_METHOD(void, unlockExtraView)(HX_THIS_(NAME) BUFVIEW view) HX_NOEXCEPT HX_PURE;\
	HX_METHOD(NAME*, lockExtraSegment)(HX_THIS_(NAME) Hx64u from, size_t size, BUFVIEW * view1, BUFVIEW * view2, Hx64u* fromActual) HX_NOEXCEPT HX_PURE;\
	HX_METHOD(NAME*, tryLockExtraSegment)(HX_THIS_(NAME) Hx64u from, size_t size, BUFVIEW * view1, BUFVIEW * view2) HX_NOEXCEPT HX_PURE;\
};\
HX_REFERENCABLE_DECLARE_END(NAME);
HX_STREAMING_BUFFER_SEGMENT_DECLARE(HxRawStreamingBufferSegment, HxRawBufConstView);
#endif //!__cplusplus

#endif //!HX_STREAMING_BUFFER_SEGMENT

#ifndef HX_STREAMING_BUFFER_SEGMENT_DEFINE

#ifdef __cplusplus
#define HX_STREAMING_BUFFER_SEGMENT_DEFINE(TYPE)\
using TYPE##StreamingBufferSegment = ::HxLibs::StreamingBufferSegment<TYPE>;
#else //__cplusplus
#define HX_STREAMING_BUFFER_SEGMENT_DEFINE(TYPE)\
HX_STREAMING_BUFFER_SEGMENT_DECLARE(TYPE##StreamingBufferSegment, TYPE##BufConstView);
#endif //__cplusplus

#endif //!HX_STREAMING_BUFFER_SEGMENT_DEFINE

#ifndef HX_STREAMING_BUFFER_SEGMENT_DEFINE_ROOT_NS

#ifdef __cplusplus
#define HX_STREAMING_BUFFER_SEGMENT_DEFINE_ROOT_NS(TYPE)\
using Hx##TYPE##StreamingBufferSegment = ::HxLibs::StreamingBufferSegment<Hx##TYPE>;\
namespace HxLibs\
{\
	using TYPE##StreamingBufferSegment = StreamingBufferSegment<TYPE>;\
}
#else //__cplusplus
#define HX_STREAMING_BUFFER_SEGMENT_DEFINE_ROOT_NS(TYPE)\
HX_STREAMING_BUFFER_SEGMENT_DECLARE(Hx##TYPE##StreamingBufferSegment, Hx##TYPE##BufConstView);
#endif //__cplusplus

#endif //!HX_STREAMING_BUFFER_SEGMENT_DEFINE_ROOT_NS

#endif //!HX_CORE_STREAMING_BUFFER_SEGMENT_H
 