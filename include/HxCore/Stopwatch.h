/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 * 
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STOPWATCH_H
#define HX_CORE_STOPWATCH_H

#ifdef __cplusplus
#ifndef HX_STOPWATCH
#define HX_STOPWATCH

#include "Clock.h"

namespace HxLibs
{
	//表示计时器，支持多次计时累加。
	template <typename Clock = SteadyClock>
	class XStopwatch
	{
	public:
		//表示时间间隔。
		using Duration = typename Clock::Duration;
		//表示时间点。
		using TimePoint = typename Clock::TimePoint;
		//表示启动标识。
		struct Starting
		{
			//NOTHING
		};

	private:
		//是否运行
		bool m_running;
		//起始位置
		TimePoint m_started;
		//时间间隔
		Duration m_duration;

	public:
		//初始化计时器。
		HX_INLINE XStopwatch() HX_NOEXCEPT
			: m_running(false)
			, m_started(Duration::zero())
			, m_duration(Duration::zero())
		{
			//NOTHING
		}
		//初始化计时器并启动计时。
		HX_INLINE XStopwatch(Starting) HX_NOEXCEPT
			: m_running(true)
			, m_started(Clock::now())
			, m_duration(Duration::zero())
		{
			//NOTHING
		}

		//获取所经历的周期计数对象。
		HX_INLINE Duration elapsed() const HX_NOEXCEPT
		{
			if (m_running)
			{
				auto current = Clock::now();
				auto duration = m_duration + (current - m_started);
				return duration;
			}
			return m_duration;
		}

		//获取是否运行。
		HX_INLINE bool running() const HX_NOEXCEPT
		{
			return m_running;
		}

		//获取所经历的以秒为单位的时间。
		HX_INLINE double elapsedSeconds() const HX_NOEXCEPT
		{
			return Clock::seconds(elapsed());
		}
		//获取所经历的以毫秒为单位的时间。
		HX_INLINE double elapsedMilliseconds() const HX_NOEXCEPT
		{
			return Clock::milliseconds(elapsed());
		}
		//获取所经历的以微妙为单位的时间。
		HX_INLINE double elapsedMicroseconds() const HX_NOEXCEPT
		{
			return Clock::microseconds(elapsed());
		}
		//获取所经历的以纳秒为单位的时间。
		HX_INLINE long long elapsedNanoseconds() const HX_NOEXCEPT
		{
			return Clock::nanoseconds(elapsed());
		}

		//开始或继续测量某个时间间隔的运行时间。
		HX_INLINE void start() HX_NOEXCEPT
		{
			if (!m_running)
			{
				m_started = Clock::now();
				m_running = true;
			}
		}

		//停止测量某个时间间隔的运行时间。
		HX_INLINE void stop() HX_NOEXCEPT
		{
			if (m_running)
			{
				auto current = Clock::now();
				m_duration += current - m_started;
				m_running = false;
			}
		}

		//停止时间间隔测量，并将运行时间重置为零。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			m_duration = Duration::zero();
			m_running = false;
		}

		//停止时间间隔测量，将运行时间重置为零，然后开始测量运行时间。
		HX_INLINE void restart() HX_NOEXCEPT
		{
			m_duration = Duration::zero();
			m_started = Clock::now();
			m_running = true;
		}

		//创建一个新计时器并开始计时。
		HX_INLINE static XStopwatch startNew() HX_NOEXCEPT
		{
			return XStopwatch(Starting{});
		}
	};

	//表示高精计时器，支持多次计时累加。
	using Stopwatch = XStopwatch<SteadyClock>;
}
using HxStopwatch = ::HxLibs::Stopwatch;

#endif //!HX_STOPWATCH
#endif //__cplusplus

#endif // !HX_CORE_STOPWATCH_H
