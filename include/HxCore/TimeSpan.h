/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_TIME_SPAN_H
#define HX_CORE_TIME_SPAN_H

#ifndef HX_TIME_SPAN
#define HX_TIME_SPAN

#ifdef __cplusplus
#include <cmath>
#include <chrono>
#include <limits.h>
#include <assert.h>
#include <type_traits>
#include "Defines.h"
static HX_CONSTEXPR long long HX_TIME_MILLISECONDS_PER_SECOND = 1000LL;
static HX_CONSTEXPR long long HX_TIME_MICROSECONDS_PER_SECOND = 1000LL * 1000LL;
static HX_CONSTEXPR long long HX_TIME_NANOSECONDS_PER_SECOND = 1000LL * 1000LL * 1000LL;
static HX_CONSTEXPR long long HX_TIME_NANOSECONDS_PER_MILLISECOND = 1000LL * 1000LL;
static HX_CONSTEXPR long long HX_TIME_NANOSECONDS_PER_MICROSECOND = 1000LL;
namespace HxLibs
{
	//表示时间间隔
	struct TimeSpan
	{
	public:
		//表示零标志
		struct Zero
		{
			//NOTHING
		};
		//表示内部值
		struct Value
		{
			//时间间隔的秒部分
			union
			{
				long long secs;
				long long seconds;
			};
			//时间间隔的纳秒部分
			union
			{
				long long nanos;
				long long nanoseconds;
			};
		};
		//表示简化标记
		struct Simplified
		{
			//NOTHING
		};

	private:
		//成员字段
		union
		{
			Value m_value;
			struct
			{
				long long m_secs;
				long long m_nanos;
			};
		};

		//执行负数验证
		HX_CONSTEXPR17_FUNC void validate() HX_NOEXCEPT
		{
			if (m_nanos < 0)
			{
				--m_secs;
				m_nanos += HX_TIME_NANOSECONDS_PER_SECOND;
			}
		}
		//转换为极简表示
		HX_CONSTEXPR17_FUNC void simplify() HX_NOEXCEPT
		{
			if (m_nanos >= HX_TIME_NANOSECONDS_PER_SECOND)
			{
				m_secs += m_nanos / HX_TIME_NANOSECONDS_PER_SECOND;
				m_nanos %= HX_TIME_NANOSECONDS_PER_SECOND;
			}
			else if (m_nanos < 0)
			{
				m_secs += m_nanos / HX_TIME_NANOSECONDS_PER_SECOND;
				m_nanos %= HX_TIME_NANOSECONDS_PER_SECOND;
				validate();
			}
		}

		//执行单次验证
		HX_CONSTEXPR17_FUNC void validateEx() HX_NOEXCEPT
		{
			if (m_nanos >= HX_TIME_NANOSECONDS_PER_SECOND)
			{
				++m_secs;
				m_nanos -= HX_TIME_NANOSECONDS_PER_SECOND;
			}
			else if (m_nanos < 0)
			{
				--m_secs;
				m_nanos += HX_TIME_NANOSECONDS_PER_SECOND;
			}
		}

	public:
		//获取内部值。
		HX_CONSTEXPR17_FUNC Value& value() HX_NOEXCEPT
		{
			return m_value;
		}
		//获取内部值。
		HX_CONSTEXPR17_FUNC const Value& value() const HX_NOEXCEPT
		{
			return m_value;
		}

		//获取秒部分。
		HX_CONSTEXPR11_FUNC long long secs() const HX_NOEXCEPT
		{
			return m_secs;
		}
		//获取秒部分。
		HX_CONSTEXPR11_FUNC long long seconds() const HX_NOEXCEPT
		{
			return m_secs;
		}

		//获取纳秒部分。
		HX_CONSTEXPR11_FUNC long long nanos() const HX_NOEXCEPT
		{
			return m_nanos;
		}
		//获取纳秒部分。
		HX_CONSTEXPR11_FUNC long long nanoseconds() const HX_NOEXCEPT
		{
			return m_nanos;
		}

		//设置秒部分。
		HX_CONSTEXPR17_FUNC void setSecs(long long value) HX_NOEXCEPT
		{
			m_secs = value;
		}
		//设置秒部分。
		HX_CONSTEXPR17_FUNC void setSeconds(long long value) HX_NOEXCEPT
		{
			m_secs = value;
		}

		//设置纳秒部分。
		HX_CONSTEXPR17_FUNC void setNanos(long long value) HX_NOEXCEPT
		{
			m_nanos = value;
			simplify();
		}
		//设置纳秒部分。
		HX_CONSTEXPR17_FUNC void setNanoseconds(long long value) HX_NOEXCEPT
		{
			m_nanos = value;
			simplify();
		}

		//设置纳秒部分。
		HX_CONSTEXPR17_FUNC void setNanos(long long value, Simplified) HX_NOEXCEPT
		{
			assert(value >= 0);
			assert(value < HX_TIME_NANOSECONDS_PER_SECOND);
			m_nanos = value;
		}
		//设置纳秒部分。
		HX_CONSTEXPR17_FUNC void setNanoseconds(long long value, Simplified) HX_NOEXCEPT
		{
			assert(value >= 0);
			assert(value < HX_TIME_NANOSECONDS_PER_SECOND);
			m_nanos = value;
		}

		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan() HX_NOEXCEPT
			: m_secs(0), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(Zero) HX_NOEXCEPT
			: m_secs(0), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(const TimeSpan& other) HX_NOEXCEPT
			: m_secs(other.m_secs), m_nanos(other.m_nanos)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(const Value& value) HX_NOEXCEPT
			: m_value(value)
		{
			simplify();
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(const Value& value, Simplified) HX_NOEXCEPT
			: m_value(value)
		{
			assert(value.nanos >= 0);
			assert(value.nanos < HX_TIME_NANOSECONDS_PER_SECOND);
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(long long secs, long long nanos) HX_NOEXCEPT
			: m_secs(secs), m_nanos(nanos)
		{
			simplify();
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(long long secs, long long nanos, Simplified) HX_NOEXCEPT
			: m_secs(secs), m_nanos(nanos)
		{
			assert(nanos >= 0);
			assert(nanos < HX_TIME_NANOSECONDS_PER_SECOND);
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(int secs) HX_NOEXCEPT
			: m_secs(secs), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(long long secs) HX_NOEXCEPT
			: m_secs(secs), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(double seconds) HX_NOEXCEPT
			: m_secs((long long)(seconds)), m_nanos((long long)((seconds - m_secs)* HX_TIME_NANOSECONDS_PER_SECOND))
		{
			validate();
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(const ::std::chrono::hours& other) HX_NOEXCEPT
			: m_secs(other.count() * 3600LL), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(const ::std::chrono::minutes& other) HX_NOEXCEPT
			: m_secs(other.count() * 60LL), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR11_FUNC TimeSpan(const ::std::chrono::seconds& other) HX_NOEXCEPT
			: m_secs(other.count()), m_nanos(0)
		{
			//NOTHING
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(const ::std::chrono::milliseconds& other) HX_NOEXCEPT
			: m_secs(other.count() / HX_TIME_MILLISECONDS_PER_SECOND), m_nanos((other.count() % HX_TIME_MILLISECONDS_PER_SECOND)* HX_TIME_NANOSECONDS_PER_MILLISECOND)
		{
			validate();
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(const ::std::chrono::microseconds& other) HX_NOEXCEPT
			: m_secs(other.count() / HX_TIME_MICROSECONDS_PER_SECOND), m_nanos((other.count() % HX_TIME_MICROSECONDS_PER_SECOND)* HX_TIME_NANOSECONDS_PER_MICROSECOND)
		{
			validate();
		}
		//初始化时间间隔
		HX_CONSTEXPR17_FUNC TimeSpan(const ::std::chrono::nanoseconds& other) HX_NOEXCEPT
			: m_secs(other.count() / HX_TIME_NANOSECONDS_PER_SECOND), m_nanos(other.count() % HX_TIME_NANOSECONDS_PER_SECOND)
		{
			validate();
		}

		//直接更改时间间隔。
		HX_INLINE void alter(Zero) HX_NOEXCEPT
		{
			m_secs = 0;
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const TimeSpan& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_secs = other.m_secs;
				m_nanos = other.m_nanos;
			}
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const Value& value) HX_NOEXCEPT
		{
			m_value = value;
			simplify();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const Value& value, Simplified) HX_NOEXCEPT
		{
			assert(value.nanos >= 0);
			assert(value.nanos < HX_TIME_NANOSECONDS_PER_SECOND);
			m_value = value;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(long long secs, long long nanos) HX_NOEXCEPT
		{
			m_secs = secs;
			m_nanos = nanos;
			simplify();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(long long secs, long long nanos, Simplified) HX_NOEXCEPT
		{
			assert(nanos >= 0);
			assert(nanos < HX_TIME_NANOSECONDS_PER_SECOND);
			m_secs = secs;
			m_nanos = nanos;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(int seconds) HX_NOEXCEPT
		{
			m_secs = seconds;
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(long long seconds) HX_NOEXCEPT
		{
			m_secs = seconds;
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(double seconds) HX_NOEXCEPT
		{
			m_secs = (long long)seconds;
			m_nanos = (long long)((seconds - m_secs) * HX_TIME_NANOSECONDS_PER_SECOND);
			validate();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::hours& other) HX_NOEXCEPT
		{
			m_secs = other.count() * 3600LL;
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::minutes& other) HX_NOEXCEPT
		{
			m_secs = other.count() * 60LL;
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::seconds& other) HX_NOEXCEPT
		{
			m_secs = other.count();
			m_nanos = 0;
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::milliseconds& other) HX_NOEXCEPT
		{
			m_secs = other.count() / HX_TIME_MILLISECONDS_PER_SECOND;
			m_nanos = (other.count() % HX_TIME_MILLISECONDS_PER_SECOND) * HX_TIME_NANOSECONDS_PER_MILLISECOND;
			validate();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::microseconds& other) HX_NOEXCEPT
		{
			m_secs = other.count() / HX_TIME_MICROSECONDS_PER_SECOND;
			m_nanos = (other.count() % HX_TIME_MICROSECONDS_PER_SECOND) * HX_TIME_NANOSECONDS_PER_MICROSECOND;
			validate();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(const ::std::chrono::nanoseconds& other) HX_NOEXCEPT
		{
			m_secs = other.count() / HX_TIME_NANOSECONDS_PER_SECOND;
			m_nanos = other.count() % HX_TIME_NANOSECONDS_PER_SECOND;
			validate();
		}

		//置为0。
		HX_INLINE TimeSpan& operator=(Zero) HX_NOEXCEPT
		{
			m_secs = 0;
			m_nanos = 0;
			return *this;
		}
		//设置为指定值。
		HX_INLINE TimeSpan& operator=(const TimeSpan& other) HX_NOEXCEPT
		{
			alter(other);
			return *this;
		}
		//设置为指定值。
		HX_INLINE TimeSpan& operator=(const Value& value) HX_NOEXCEPT
		{
			alter(value);
			return *this;
		}
		//设置为指定值。
		HX_INLINE TimeSpan& operator=(int seconds) HX_NOEXCEPT
		{
			alter(seconds);
			return *this;
		}
		//设置为指定值。
		HX_INLINE TimeSpan& operator=(long long seconds) HX_NOEXCEPT
		{
			alter(seconds);
			return *this;
		}
		//设置为指定值。
		HX_INLINE TimeSpan& operator=(double seconds) HX_NOEXCEPT
		{
			alter(seconds);
			return *this;
		}
		//设置为指定值。
		template <typename Rep, typename Period>
		HX_INLINE TimeSpan& operator=(const ::std::chrono::duration<Rep, Period>& other) HX_NOEXCEPT
		{
			alter(other);
			return *this;
		}

		//判断当前时间间隔是否为零值。
		HX_CONSTEXPR11_FUNC bool equals(Zero) const HX_NOEXCEPT
		{
			return m_secs == 0 && m_nanos == 0;
		}
		//判断当前时间间隔和指定时间间隔是否相同。
		HX_CONSTEXPR11_FUNC bool equals(const TimeSpan& other) const HX_NOEXCEPT
		{
			return m_secs == other.m_secs && m_nanos == other.m_nanos;
		}
		//判断当前时间间隔和指定时间间隔是否相同。
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool equals(const ::std::chrono::duration<Rep, Period>& other) const HX_NOEXCEPT
		{
			return equals(TimeSpan(other));
		}

		//OPERATOR <
		HX_CONSTEXPR11_FUNC bool operator<(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs < right.m_secs || (m_secs == right.m_secs && m_nanos < right.m_nanos);
		}
		//OPERATOR <
		HX_CONSTEXPR11_FUNC bool operator<(int right) const HX_NOEXCEPT
		{
			return operator<(TimeSpan(right));
		}
		//OPERATOR <
		HX_CONSTEXPR11_FUNC bool operator<(long long right) const HX_NOEXCEPT
		{
			return operator<(TimeSpan(right));
		}
		//OPERATOR <
		HX_CONSTEXPR17_FUNC bool operator<(double right) const HX_NOEXCEPT
		{
			return operator<(TimeSpan(right));
		}
		//OPERATOR <
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator<(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator<(TimeSpan(right));
		}

		//OPERATOR <=
		HX_CONSTEXPR11_FUNC bool operator<=(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs < right.m_secs || (m_secs == right.m_secs && m_nanos <= right.m_nanos);
		}
		//OPERATOR <=
		HX_CONSTEXPR11_FUNC bool operator<=(int right) const HX_NOEXCEPT
		{
			return operator<=(TimeSpan(right));
		}
		//OPERATOR <=
		HX_CONSTEXPR11_FUNC bool operator<=(long long right) const HX_NOEXCEPT
		{
			return operator<=(TimeSpan(right));
		}
		//OPERATOR <=
		HX_CONSTEXPR17_FUNC bool operator<=(double right) const HX_NOEXCEPT
		{
			return operator<=(TimeSpan(right));
		}
		//OPERATOR <=
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator<=(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator<=(TimeSpan(right));
		}

		//OPERATOR >
		HX_CONSTEXPR11_FUNC bool operator>(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs > right.m_secs || (m_secs == right.m_secs && m_nanos > right.m_nanos);
		}
		//OPERATOR >
		HX_CONSTEXPR11_FUNC bool operator>(int right) const HX_NOEXCEPT
		{
			return operator>(TimeSpan(right));
		}
		//OPERATOR >
		HX_CONSTEXPR11_FUNC bool operator>(long long right) const HX_NOEXCEPT
		{
			return operator>(TimeSpan(right));
		}
		//OPERATOR >
		HX_CONSTEXPR17_FUNC bool operator>(double right) const HX_NOEXCEPT
		{
			return operator>(TimeSpan(right));
		}
		//OPERATOR >
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator>(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator>(TimeSpan(right));
		}

		//OPERATOR >=
		HX_CONSTEXPR11_FUNC bool operator>=(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs > right.m_secs || (m_secs == right.m_secs && m_nanos >= right.m_nanos);
		}
		//OPERATOR >=
		HX_CONSTEXPR11_FUNC bool operator>=(int right) const HX_NOEXCEPT
		{
			return operator>=(TimeSpan(right));
		}
		//OPERATOR >=
		HX_CONSTEXPR11_FUNC bool operator>=(long long right) const HX_NOEXCEPT
		{
			return operator>=(TimeSpan(right));
		}
		//OPERATOR >=
		HX_CONSTEXPR17_FUNC bool operator>=(double right) const HX_NOEXCEPT
		{
			return operator>=(TimeSpan(right));
		}
		//OPERATOR >=
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator>=(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator>=(TimeSpan(right));
		}

		//OPERATOR ==
		HX_CONSTEXPR11_FUNC bool operator==(const Zero& right) const HX_NOEXCEPT
		{
			return m_secs == 0 && m_nanos == 0;
		}
		//OPERATOR ==
		HX_CONSTEXPR11_FUNC bool operator==(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs == right.m_secs && m_nanos == right.m_nanos;
		}
		//OPERATOR ==
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator==(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator==(TimeSpan(right));
		}

		//OPERATOR !=
		HX_CONSTEXPR11_FUNC bool operator!=(const Zero& right) const HX_NOEXCEPT
		{
			return m_secs != 0 || m_nanos != 0;
		}
		//OPERATOR !=
		HX_CONSTEXPR11_FUNC bool operator!=(const TimeSpan& right) const HX_NOEXCEPT
		{
			return m_secs != right.m_secs || m_nanos != right.m_nanos;
		}
		//OPERATOR !=
		template <typename Rep, typename Period>
		HX_CONSTEXPR11_FUNC bool operator!=(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator!=(TimeSpan(right));
		}

		//OPERATOR +
		HX_INLINE TimeSpan operator+() const HX_NOEXCEPT
		{
			return *this;
		}
		//OPERATOR -
		HX_INLINE TimeSpan operator-() const HX_NOEXCEPT
		{
			TimeSpan t(-m_secs, -m_nanos, Simplified{});
			if (t.m_nanos < 0)
			{
				--t.m_secs;
				t.m_nanos += HX_TIME_NANOSECONDS_PER_SECOND;
			}
			return t;
		}

		//OPERATOR +=
		HX_INLINE TimeSpan& operator+=(const TimeSpan& right) HX_NOEXCEPT
		{
			m_secs += right.m_secs;
			m_nanos += right.m_nanos;
			validateEx();
			return *this;
		}
		//OPERATOR +=
		HX_INLINE TimeSpan& operator+=(double right) HX_NOEXCEPT
		{
			return operator+=(TimeSpan(right));
		}
		//OPERATOR +=
		template <typename Rep, typename Period>
		HX_INLINE TimeSpan& operator+=(const ::std::chrono::duration<Rep, Period>& right) HX_NOEXCEPT
		{
			return operator+=(TimeSpan(right));
		}

		//OPERATOR +
		HX_INLINE TimeSpan operator+(const TimeSpan& right) const HX_NOEXCEPT
		{
			auto t(*this);
			t += right;
			return t;
		}
		//OPERATOR +
		HX_INLINE TimeSpan operator+(double right) const HX_NOEXCEPT
		{
			return operator+(TimeSpan(right));
		}
		//OPERATOR +
		template <typename Rep, typename Period>
		HX_INLINE TimeSpan operator+(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator+(TimeSpan(right));
		}

		//OPERATOR -=
		HX_INLINE TimeSpan& operator-=(const TimeSpan& right) HX_NOEXCEPT
		{
			m_secs -= right.m_secs;
			m_nanos -= right.m_nanos;
			validateEx();
			return *this;
		}
		//OPERATOR -=
		HX_INLINE TimeSpan& operator-=(double right) HX_NOEXCEPT
		{
			return operator-=(TimeSpan(right));
		}
		//OPERATOR -=
		template <typename Rep, typename Period>
		HX_INLINE TimeSpan& operator-=(const ::std::chrono::duration<Rep, Period>& right) HX_NOEXCEPT
		{
			return operator-=(TimeSpan(right));
		}

		//OPERATOR -
		HX_INLINE TimeSpan operator-(const TimeSpan& right) const HX_NOEXCEPT
		{
			auto t(*this);
			t -= right;
			return t;
		}
		//OPERATOR -
		HX_INLINE TimeSpan operator-(double right) const HX_NOEXCEPT
		{
			return operator-(TimeSpan(right));
		}
		//OPERATOR -
		template <typename Rep, typename Period>
		HX_INLINE TimeSpan operator-(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator-(TimeSpan(right));
		}

		//获取以秒为单位的浮点数
		HX_CONSTEXPR11_FUNC double totalSeconds() const HX_NOEXCEPT
		{
			return (double)m_secs + m_nanos / 1e9;
		}
		//获取以毫秒为单位的浮点数
		HX_CONSTEXPR11_FUNC double totalMilliseconds() const HX_NOEXCEPT
		{
			return (double)m_secs * 1e3 + m_nanos / 1e6;
		}
		//获取以微秒为单位的浮点数
		HX_CONSTEXPR11_FUNC double totalMicroseconds() const HX_NOEXCEPT
		{
			return (double)m_secs * 1e6 + m_nanos / 1e3;
		}

		//OPERATOR double（秒数）
		HX_CONSTEXPR11_FUNC operator double() const HX_NOEXCEPT
		{
			return totalSeconds();
		}

		//计算在指定采样率下的样本数（浮点）。
		HX_INLINE double sampleOffset(double sampleRate) const HX_NOEXCEPT
		{
			return m_secs * sampleRate + m_nanos * sampleRate / 1e9;
		}
		//计算在指定采样率下的样本数（浮点）。
		HX_INLINE double sampleOffset(long long sampleRate) const HX_NOEXCEPT
		{
			return m_secs * sampleRate + m_nanos * sampleRate / 1e9;
		}
		//计算在指定采样率下的样本数（浮点）。
		template <typename TSampleRate>
		HX_INLINE double sampleOffset(TSampleRate sampleRate) const HX_NOEXCEPT;

		//计算在指定采样率下的样本数（整数）。
		HX_INLINE long long sampleCount(double sampleRate) const HX_NOEXCEPT
		{
			return (long long)::std::round(m_secs * sampleRate + m_nanos * sampleRate / 1e9);
		}
		//计算在指定采样率下的样本数（整数）。
		HX_INLINE long long sampleCount(long long sampleRate) const HX_NOEXCEPT
		{
			return m_secs * sampleRate + (m_nanos * sampleRate + HX_TIME_NANOSECONDS_PER_SECOND / 2) / HX_TIME_NANOSECONDS_PER_SECOND;
		}
		//计算在指定采样率下的样本数（整数）。
		template <typename TSampleRate>
		HX_INLINE long long sampleCount(TSampleRate sampleRate) const HX_NOEXCEPT;

		//根据样点和采样率计算时间间隔
		HX_INLINE static TimeSpan make(double sampleRate, double sampleCount) HX_NOEXCEPT
		{
			return TimeSpan(sampleCount / sampleRate);
		}
		//根据样点和采样率计算时间间隔
		HX_INLINE static TimeSpan make(double sampleRate, long long sampleCount) HX_NOEXCEPT
		{
			if (sampleCount != 0)
			{
				return TimeSpan(sampleCount / sampleRate);
			}
			return TimeSpan();
		}
		//根据样点和采样率计算时间间隔
		HX_INLINE static TimeSpan make(long long sampleRate, long long sampleCount) HX_NOEXCEPT
		{
			if (sampleCount != 0)
			{
				auto secs = sampleCount / sampleRate;
				auto nanos = ((sampleCount % sampleRate) * HX_TIME_NANOSECONDS_PER_SECOND + HX_TIME_NANOSECONDS_PER_SECOND / 2) / sampleRate;
				if (nanos < 0)
				{
					--secs;
					nanos += HX_TIME_NANOSECONDS_PER_SECOND;
				}
				return TimeSpan(secs, nanos, Simplified{});
			}
			return TimeSpan();
		}
		//根据样点和采样率计算时间间隔
		HX_INLINE static TimeSpan make(long long sampleRate, double sampleCount) HX_NOEXCEPT
		{
			long long secs = 0;
			auto sampleCountLL = (long long)sampleCount;
			if (sampleCountLL != 0)
			{
				secs = sampleCountLL / sampleRate;
				sampleCount -= sampleCountLL - sampleCountLL % sampleRate;
			}
			TimeSpan t(sampleCount / sampleRate);
			t.m_secs += secs;
			return t;
		}
		//根据样点和采样率计算时间间隔
		template <typename TSampleRate, typename TSampleCount>
		HX_INLINE static TimeSpan make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT;
	};

	//OPERATOR <
	HX_INLINE bool operator<(int left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right >= left;
	}
	//OPERATOR <
	HX_INLINE bool operator<(long long left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right >= left;
	}
	//OPERATOR <
	HX_INLINE bool operator<(double left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right >= left;
	}
	//OPERATOR <
	template <typename Rep, typename Period>
	HX_INLINE bool operator<(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right >= left;
	}

	//OPERATOR <=
	HX_INLINE bool operator<=(int left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right > left;
	}
	//OPERATOR <=
	HX_INLINE bool operator<=(long long left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right > left;
	}
	//OPERATOR <=
	HX_INLINE bool operator<=(double left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right > left;
	}
	//OPERATOR <=
	template <typename Rep, typename Period>
	HX_INLINE bool operator<=(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right > left;
	}

	//OPERATOR >
	HX_INLINE bool operator>(int left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right <= left;
	}
	//OPERATOR >
	HX_INLINE bool operator>(long long left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right <= left;
	}
	//OPERATOR >
	HX_INLINE bool operator>(double left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right <= left;
	}
	//OPERATOR >
	template <typename Rep, typename Period>
	HX_INLINE bool operator>(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right <= left;
	}

	//OPERATOR >=
	HX_INLINE bool operator>=(int left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right < left;
	}
	//OPERATOR >=
	HX_INLINE bool operator>=(long long left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right < left;
	}
	//OPERATOR >=
	HX_INLINE bool operator>=(double left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right < left;
	}
	//OPERATOR >=
	template <typename Rep, typename Period>
	HX_INLINE bool operator>=(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right < left;
	}

	//OPERATOR ==
	HX_INLINE bool operator==(const TimeSpan::Zero& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right == left;
	}
	//OPERATOR ==
	template <typename Rep, typename Period>
	HX_INLINE bool operator==(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right == left;
	}

	//OPERATOR ==
	HX_INLINE bool operator!=(const TimeSpan::Zero& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right != left;
	}
	//OPERATOR !=
	template <typename Rep, typename Period>
	HX_INLINE bool operator!=(const ::std::chrono::duration<Rep, Period>& left, const TimeSpan& right) HX_NOEXCEPT
	{
		return right != left;
	}

	template <typename TSampleRate, bool>
	struct __TimeSpanUtil1__;

	template <typename TSampleRate>
	struct __TimeSpanUtil1__<TSampleRate, true>
	{
		HX_INLINE static double sampleOffset(const TimeSpan& obj, TSampleRate arg) HX_NOEXCEPT
		{
			return obj.sampleOffset((long long)arg);
		}
		HX_INLINE static long long sampleCount(const TimeSpan& obj, TSampleRate arg) HX_NOEXCEPT
		{
			return obj.sampleCount((long long)arg);
		}
	};
	template <typename TSampleRate>
	struct __TimeSpanUtil1__<TSampleRate, false>
	{
		HX_INLINE static double sampleOffset(const TimeSpan& obj, TSampleRate arg) HX_NOEXCEPT
		{
			return obj.sampleOffset((double)arg);
		}
		HX_INLINE static long long sampleCount(const TimeSpan& obj, TSampleRate arg) HX_NOEXCEPT
		{
			return obj.sampleCount((double)arg);
		}
	};

	template <typename TSampleRate>
	HX_INLINE double TimeSpan::sampleOffset(TSampleRate sampleRate) const HX_NOEXCEPT
	{
		return __TimeSpanUtil1__<TSampleRate, ::std::is_integral<TSampleRate>::value> ::sampleOffset(*this, sampleRate);
	}
	template <typename TSampleRate>
	HX_INLINE long long TimeSpan::sampleCount(TSampleRate sampleRate) const HX_NOEXCEPT
	{
		return __TimeSpanUtil1__<TSampleRate, ::std::is_integral<TSampleRate>::value> ::sampleCount(*this, sampleRate);
	}

	template <typename TSampleRate, bool, typename TSampleCount, bool>
	struct __TimeSpanUtil2__;

	template <typename TSampleRate, typename TSampleCount>
	struct __TimeSpanUtil2__<TSampleRate, true, TSampleCount, true>
	{
		HX_INLINE static TimeSpan make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return TimeSpan::make((long long)sampleRate, (long long)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeSpanUtil2__<TSampleRate, true, TSampleCount, false>
	{
		HX_INLINE static TimeSpan make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return TimeSpan::make((long long)sampleRate, (double)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeSpanUtil2__<TSampleRate, false, TSampleCount, true>
	{
		HX_INLINE static TimeSpan make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return TimeSpan::make((double)sampleRate, (long long)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeSpanUtil2__<TSampleRate, false, TSampleCount, false>
	{
		HX_INLINE static TimeSpan make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return TimeSpan::make((double)sampleRate, (double)sampleCount);
		}
	};

	template <typename TSampleRate, typename TSampleCount>
	HX_INLINE TimeSpan TimeSpan::make(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
	{
		return __TimeSpanUtil2__<TSampleRate, ::std::is_integral<TSampleRate>::value, TSampleCount, ::std::is_integral<TSampleCount>::value > ::make(sampleRate, sampleCount);
	}
}
using HxTimeSpan = ::HxLibs::TimeSpan;
#else //!__cplusplus
#define HX_TIME_MILLISECONDS_PER_SECOND (1000LL)
#define HX_TIME_MICROSECONDS_PER_SECOND (1000LL * 1000LL)
#define HX_TIME_NANOSECONDS_PER_SECOND (1000LL * 1000LL * 1000LL)
#define HX_TIME_NANOSECONDS_PER_MILLISECOND (1000LL * 1000LL);
#define HX_TIME_NANOSECONDS_PER_MICROSECOND (1000LL);
typedef struct HxTimeSpan
{
	union
	{
		long long secs;
		long long seconds;
	};
	union
	{
		long long nanos;
		long long nanoseconds;
	};
}HxTimeSpan;
#endif //!__cplusplus

#endif //!HX_TIME_SPAN

#endif //!HX_CORE_TIME_SPAN_H
