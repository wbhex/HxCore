/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CLOCK_H
#define HX_CORE_CLOCK_H

#ifdef __cplusplus

#ifndef HX_CLOCK
#define HX_CLOCK

#include <chrono>
#include "Defines.h"

namespace HxLibs
{
	template <typename Impl>
	struct Clock
	{
		//表示时间间隔。
		using Duration = typename Impl::duration;
		//表示时间点。
		using TimePoint = typename Impl::time_point;

		//获取当前时间点。
		HX_INLINE static TimePoint now() HX_NOEXCEPT
		{
			return Impl::now();
		}

		//获取以秒为单位的时间间隔。
		HX_INLINE static double seconds(const Duration& duration) HX_NOEXCEPT
		{
			return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(duration).count() / 1e9;
		}
		//获取以毫秒为单位的时间间隔。
		HX_INLINE static double milliseconds(const Duration& duration) HX_NOEXCEPT
		{
			return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(duration).count() / 1e6;
		}
		//获取以微妙为单位的时间间隔。
		HX_INLINE static double microseconds(const Duration& duration) HX_NOEXCEPT
		{
			return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(duration).count() / 1e3;
		}
		//获取以纳秒为单位的时间间隔。
		HX_INLINE static long long nanoseconds(const Duration& duration) HX_NOEXCEPT
		{
			return ::std::chrono::duration_cast<::std::chrono::nanoseconds>(duration).count();
		}

		//获取指定起始点与当前时间点的时间间隔。
		HX_INLINE static Duration elapsed(const TimePoint& started) HX_NOEXCEPT
		{
			return now() - started;
		}
		//获取指定起始点与当前时间点所经历的以秒为单位的时间间隔。
		HX_INLINE static double elapsedSeconds(const TimePoint& started) HX_NOEXCEPT
		{
			return seconds(elapsed(started));
		}
		//获取指定起始点与当前时间点所经历的以毫秒为单位的时间间隔。
		HX_INLINE static double elapsedMilliseconds(const TimePoint& started) HX_NOEXCEPT
		{
			return milliseconds(elapsed(started));
		}
		//获取指定起始点与当前时间点所经历的以微妙为单位的时间间隔。
		HX_INLINE static double elapsedMicroseconds(const TimePoint& started) HX_NOEXCEPT
		{
			return microseconds(elapsed(started));
		}
		//获取指定起始点与当前时间点所经历的以纳秒为单位的时间间隔。
		HX_INLINE static long long elapsedNanoseconds(const TimePoint& started) HX_NOEXCEPT
		{
			return nanoseconds(elapsed(started));
		}
	};
}

#endif //!HX_CLOCK

#ifndef HX_STEADY_CLOCK
#define HX_STEADY_CLOCK

namespace HxLibs
{
	struct SteadyClock : public Clock<::std::chrono::steady_clock> {};
}
using HxSteadyClock = ::HxLibs::SteadyClock;

#endif //!HX_STEADY_CLOCK

#ifndef HX_SYSTEM_CLOCK
#define HX_SYSTEM_CLOCK

namespace HxLibs
{
	struct SystemClock : public Clock<::std::chrono::system_clock> {};
}
using HxSystemClock = ::HxLibs::SystemClock;

#endif //!HX_SYSTEM_CLOCK

#endif //__cplusplus

#endif //!HX_CORE_CLOCK_H
