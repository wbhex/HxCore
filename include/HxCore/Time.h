/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_TIME_H
#define HX_CORE_TIME_H

#ifndef HX_TIME
#define HX_TIME

#include "Struct.h"
#include "TimeSpan.h"
#ifdef __cplusplus
#include <ctime>
#include <functional>
#include "HashUtils.h"
namespace HxLibs
{
	//表示日期时间
	struct Time
	{
	public:
		//表示当前时间标志
		struct Now
		{
			//NOTHING
		};
		//表示零标志
		struct Zero
		{
			//NOTHING
		};
		//表示内部值
		struct Value
		{
			//时间的秒部分
			union
			{
				time_t secs;
				time_t seconds;
			};
			//时间的纳秒部分
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
				time_t m_secs;
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
		HX_CONSTEXPR11_FUNC time_t secs() const HX_NOEXCEPT
		{
			return m_secs;
		}
		//获取秒部分。
		HX_CONSTEXPR11_FUNC time_t seconds() const HX_NOEXCEPT
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
		HX_CONSTEXPR17_FUNC void setSecs(time_t value) HX_NOEXCEPT
		{
			m_secs = value;
		}
		//设置秒部分。
		HX_CONSTEXPR17_FUNC void setSeconds(time_t value) HX_NOEXCEPT
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

		//初始化日期时间
		HX_CONSTEXPR11_FUNC Time() HX_NOEXCEPT
			: m_secs(0), m_nanos(0)
		{
			//NOTHING
		}
		//初始化当前时间。
		HX_INLINE Time(Now) HX_NOEXCEPT
		{
			auto timePoint = ::std::chrono::system_clock::now();
			m_secs = ::std::chrono::system_clock::to_time_t(timePoint);
			m_nanos = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(timePoint - ::std::chrono::system_clock::from_time_t(m_secs)).count();
		}
		//初始化日期时间
		HX_CONSTEXPR11_FUNC Time(Zero) HX_NOEXCEPT
			: m_secs(0), m_nanos(0)
		{
			//NOTHING
		}
		//初始化日期时间
		HX_CONSTEXPR11_FUNC Time(const Time& other) HX_NOEXCEPT
			: m_secs(other.m_secs), m_nanos(other.m_nanos)
		{
			//NOTHING
		}
		//初始化日期时间
		HX_CONSTEXPR17_FUNC Time(const Value& value) HX_NOEXCEPT
			: m_value(value)
		{
			simplify();
		}
		//初始化日期时间
		HX_CONSTEXPR17_FUNC Time(const Value& value, Simplified) HX_NOEXCEPT
			: m_value(value)
		{
			assert(value.nanos >= 0);
			assert(value.nanos < HX_TIME_NANOSECONDS_PER_SECOND);
		}
		//初始化日期时间
		HX_CONSTEXPR11_FUNC Time(time_t secs) HX_NOEXCEPT
			: m_secs(secs), m_nanos(0)
		{
			//NOTHING
		}
		//初始化日期时间
		HX_CONSTEXPR17_FUNC Time(time_t secs, long long nanos) HX_NOEXCEPT
			: m_secs(secs), m_nanos(nanos)
		{
			simplify();
		}
		//初始化日期时间
		HX_CONSTEXPR17_FUNC Time(time_t secs, long long nanos, Simplified) HX_NOEXCEPT
			: m_secs(secs), m_nanos(nanos)
		{
			assert(nanos >= 0);
			assert(nanos < HX_TIME_NANOSECONDS_PER_SECOND);
		}

		//获取当前时间
		HX_INLINE static Time now() HX_NOEXCEPT
		{
			auto timePoint = ::std::chrono::system_clock::now();
			auto secs = ::std::chrono::system_clock::to_time_t(timePoint);
			auto nanos = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(timePoint - ::std::chrono::system_clock::from_time_t(secs)).count();
			return Time(secs, nanos, Simplified{});
		}

		//置为0。
		HX_INLINE void zero() HX_NOEXCEPT
		{
			m_secs = 0;
			m_nanos = 0;
		}

		//直接更改日期时间。
		HX_INLINE void alter(Zero) HX_NOEXCEPT
		{
			m_secs = 0;
			m_nanos = 0;
		}
		//直接更改日期时间。
		HX_INLINE void alter(const Time& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_secs = other.m_secs;
				m_nanos = other.m_nanos;
			}
		}
		//直接更改日期时间。
		HX_INLINE void alter(const Value& value) HX_NOEXCEPT
		{
			m_value = value;
			simplify();
		}
		//直接更改日期时间。
		HX_INLINE void alter(const Value& value, Simplified) HX_NOEXCEPT
		{
			assert(value.nanos >= 0);
			assert(value.nanos < HX_TIME_NANOSECONDS_PER_SECOND);
			m_value = value;
		}
		//直接更改日期时间。
		HX_INLINE void alter(time_t secs) HX_NOEXCEPT
		{
			m_secs = secs;
			m_nanos = 0;
		}
		//直接更改日期时间。
		HX_INLINE void alter(time_t secs, long long nanos) HX_NOEXCEPT
		{
			m_secs = secs;
			m_nanos = nanos;
			simplify();
		}
		//直接更改时间间隔。
		HX_INLINE void alter(time_t secs, long long nanos, Simplified) HX_NOEXCEPT
		{
			assert(nanos >= 0);
			assert(nanos < HX_TIME_NANOSECONDS_PER_SECOND);
			m_secs = secs;
			m_nanos = nanos;
		}

		//重置为当前时间。
		HX_INLINE Time& operator=(Now) HX_NOEXCEPT
		{
			auto timePoint = ::std::chrono::system_clock::now();
			m_secs = ::std::chrono::system_clock::to_time_t(timePoint);
			m_nanos = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(timePoint - ::std::chrono::system_clock::from_time_t(m_secs)).count();
			return *this;
		}
		//置为0。
		HX_INLINE Time& operator=(Zero) HX_NOEXCEPT
		{
			m_secs = 0;
			m_nanos = 0;
			return *this;
		}
		//设置为指定值。
		HX_INLINE Time& operator=(const Value& value) HX_NOEXCEPT
		{
			alter(value);
			return *this;
		}
		//设置为指定值。
		HX_INLINE Time& operator=(const Time& other) HX_NOEXCEPT
		{
			alter(other);
			return *this;
		}

		//判断当前日期时间是否为零值。
		HX_CONSTEXPR11_FUNC bool equals(Zero) const HX_NOEXCEPT
		{
			return m_secs == 0 && m_nanos == 0;
		}
		//判断当前日期时间和指定日期时间是否相同。
		HX_CONSTEXPR11_FUNC bool equals(const Time& other) const HX_NOEXCEPT
		{
			return m_secs == other.m_secs && m_nanos == other.m_nanos;
		}

		//更新日期时间，如果发生改变则返回True。
		HX_INLINE bool update(Zero) HX_NOEXCEPT
		{
			bool changed = false;
			if (m_secs != 0)
			{
				changed = true;
				m_secs = 0;
			}
			if (m_nanos != 0)
			{
				changed = true;
				m_nanos = 0;
			}
			return changed;
		}
		//更新日期时间，如果发生改变则返回True。
		HX_INLINE bool update(time_t secs, long long nanos) HX_NOEXCEPT
		{
			Time value(secs, nanos);
			bool changed = false;
			if (m_secs != value.m_secs)
			{
				changed = true;
				m_secs = value.m_secs;
			}
			if (m_nanos != value.m_nanos)
			{
				changed = true;
				m_nanos = value.m_nanos;
			}
			return changed;
		}
		//更新日期时间，如果发生改变则返回True。
		HX_INLINE bool update(time_t secs, long long nanos, Simplified) HX_NOEXCEPT
		{
			assert(nanos >= 0);
			assert(nanos < HX_TIME_NANOSECONDS_PER_SECOND);
			bool changed = false;
			if (m_secs != secs)
			{
				changed = true;
				m_secs = secs;
			}
			if (m_nanos != nanos)
			{
				changed = true;
				m_nanos = nanos;
			}
			return changed;
		}

		//OPERATOR <
		HX_CONSTEXPR11_FUNC bool operator<(const Time& right) const HX_NOEXCEPT
		{
			return m_secs < right.m_secs || (m_secs == right.m_secs && m_nanos < right.m_nanos);
		}
		//OPERATOR <=
		HX_CONSTEXPR11_FUNC bool operator<=(const Time& right) const HX_NOEXCEPT
		{
			return m_secs < right.m_secs || (m_secs == right.m_secs && m_nanos <= right.m_nanos);
		}
		//OPERATOR >
		HX_CONSTEXPR11_FUNC bool operator>(const Time& right) const HX_NOEXCEPT
		{
			return m_secs > right.m_secs || (m_secs == right.m_secs && m_nanos > right.m_nanos);
		}
		//OPERATOR >=
		HX_CONSTEXPR11_FUNC bool operator>=(const Time& right) const HX_NOEXCEPT
		{
			return m_secs > right.m_secs || (m_secs == right.m_secs && m_nanos >= right.m_nanos);
		}
		//OPERATOR ==
		HX_CONSTEXPR11_FUNC bool operator==(const Time& right) const HX_NOEXCEPT
		{
			return m_secs == right.m_secs && m_nanos == right.m_nanos;
		}
		//OPERATOR !=
		HX_CONSTEXPR11_FUNC bool operator!=(const Time& right) const HX_NOEXCEPT
		{
			return m_secs != right.m_secs || m_nanos != right.m_nanos;
		}

		//OPERATOR -
		HX_CONSTEXPR17_FUNC TimeSpan operator-(const Time& right) const HX_NOEXCEPT
		{
			return TimeSpan(m_secs - right.m_secs, m_nanos - right.m_nanos);
		}

		//OPERATOR -
		HX_CONSTEXPR17_FUNC Time& operator-=(const TimeSpan& right) HX_NOEXCEPT
		{
			m_secs -= right.secs();
			m_nanos -= right.nanos();
			validateEx();
			return *this;
		}
		//OPERATOR -
		HX_CONSTEXPR17_FUNC Time& operator-=(double right) HX_NOEXCEPT
		{
			return operator-=(TimeSpan(right));
		}
		//OPERATOR -
		template <typename Rep, typename Period>
		HX_CONSTEXPR17_FUNC Time& operator-=(const ::std::chrono::duration<Rep, Period>& right) HX_NOEXCEPT
		{
			return operator-=(TimeSpan(right));
		}

		//OPERATOR -
		HX_CONSTEXPR17_FUNC Time operator-(const TimeSpan& right) const HX_NOEXCEPT
		{
			return Time(m_secs - right.secs(), m_nanos - right.nanos());
		}
		//OPERATOR -
		HX_CONSTEXPR17_FUNC Time operator-(double right) const HX_NOEXCEPT
		{
			return operator-(TimeSpan(right));
		}
		//OPERATOR -
		template <typename Rep, typename Period>
		HX_CONSTEXPR17_FUNC Time operator-(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator-(TimeSpan(right));
		}

		//OPERATOR +
		HX_CONSTEXPR17_FUNC Time& operator+=(const TimeSpan& right) HX_NOEXCEPT
		{
			m_secs += right.secs();
			m_nanos += right.nanos();
			validateEx();
			return *this;
		}
		//OPERATOR +
		HX_CONSTEXPR17_FUNC Time& operator+=(double right) HX_NOEXCEPT
		{
			return operator+=(TimeSpan(right));
		}
		//OPERATOR +
		template <typename Rep, typename Period>
		HX_CONSTEXPR17_FUNC Time& operator+=(const ::std::chrono::duration<Rep, Period>& right) HX_NOEXCEPT
		{
			return operator+=(TimeSpan(right));
		}

		//OPERATOR +
		HX_CONSTEXPR17_FUNC Time operator+(const TimeSpan& right) const HX_NOEXCEPT
		{
			return Time(m_secs + right.secs(), m_nanos + right.nanos());
		}
		//OPERATOR +
		HX_CONSTEXPR17_FUNC Time operator+(double right) const HX_NOEXCEPT
		{
			return operator+(TimeSpan(right));
		}
		//OPERATOR +
		template <typename Rep, typename Period>
		HX_CONSTEXPR17_FUNC Time operator+(const ::std::chrono::duration<Rep, Period>& right) const HX_NOEXCEPT
		{
			return operator+(TimeSpan(right));
		}

		//表示哈希计算
		struct Hash
		{
			HX_CONSTEXPR17_FUNC size_t operator()(const Time& time) const HX_NOEXCEPT
			{
				return HxHash_fnv1a((const unsigned char*)&time, sizeof(Time));
			}
		};

		//转换为时间信息
		HX_INLINE::tm* gmtime() const HX_NOEXCEPT
		{
			return ::gmtime(&m_secs);
		}
		//转换为本地时间信息
		HX_INLINE::tm* localtime() const HX_NOEXCEPT
		{
			return ::localtime(&m_secs);
		}

		//根据样点和采样率更新时间
		HX_INLINE void updateTime(double sampleRate, double sampleCount) HX_NOEXCEPT
		{
			operator+=(sampleCount / sampleRate);
		}
		//根据样点和采样率更新时间
		HX_INLINE void updateTime(double sampleRate, long long sampleCount) HX_NOEXCEPT
		{
			if (sampleCount != 0)
			{
				operator+=(sampleCount / sampleRate);
			}
		}
		//根据样点和采样率更新时间
		HX_INLINE void updateTime(long long sampleRate, long long sampleCount) HX_NOEXCEPT
		{
			if (sampleCount != 0)
			{
				m_secs += sampleCount / sampleRate;
				m_nanos += ((sampleCount % sampleRate) * HX_TIME_NANOSECONDS_PER_SECOND + HX_TIME_NANOSECONDS_PER_SECOND / 2) / sampleRate;
				validateEx();
			}
		}
		//根据样点和采样率更新时间
		HX_INLINE void updateTime(long long sampleRate, double sampleCount) HX_NOEXCEPT
		{
			auto sampleCountLL = (long long)sampleCount;
			if (sampleCountLL != 0)
			{
				m_secs += sampleCountLL / sampleRate;
				sampleCount -= sampleCountLL - sampleCountLL % sampleRate;
			}
			operator+=(sampleCount / sampleRate);
		}
		//根据样点和采样率更新时间
		template <typename TSampleRate, typename TSampleCount>
		HX_INLINE void updateTime(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT;

		//根据样点和采样率创建时间
		HX_INLINE Time createTime(double sampleRate, double sampleCount) const HX_NOEXCEPT
		{
			return operator+(sampleCount / sampleRate);
		}
		//根据样点和采样率创建时间
		HX_INLINE Time createTime(double sampleRate, long long sampleCount) const HX_NOEXCEPT
		{
			if (sampleCount != 0)
			{
				return operator+(sampleCount / sampleRate);
			}
			return *this;
		}
		//根据样点和采样率创建时间
		HX_INLINE Time createTime(long long sampleRate, long long sampleCount) const HX_NOEXCEPT
		{
			auto t = *this;
			t.updateTime(sampleRate, sampleCount);
			return t;
		}
		//根据样点和采样率创建时间
		HX_INLINE Time createTime(long long sampleRate, double sampleCount) const HX_NOEXCEPT
		{
			auto t = *this;
			t.updateTime(sampleRate, sampleCount);
			return t;
		}
		//根据样点和采样率创建时间
		template <typename TSampleRate, typename TSampleCount>
		HX_INLINE Time createTime(TSampleRate sampleRate, TSampleCount sampleCount) const HX_NOEXCEPT;
	};

	template <typename TSampleRate, bool, typename TSampleCount, bool>
	struct __TimeUtil2__;

	template <typename TSampleRate, typename TSampleCount>
	struct __TimeUtil2__<TSampleRate, true, TSampleCount, true>
	{
		HX_INLINE static void updateTime(Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			obj.updateTime((long long)sampleRate, (long long)sampleCount);
		}
		HX_INLINE static Time createTime(const Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return obj.createTime((long long)sampleRate, (long long)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeUtil2__<TSampleRate, true, TSampleCount, false>
	{
		HX_INLINE static void updateTime(Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			obj.updateTime((long long)sampleRate, (double)sampleCount);
		}
		HX_INLINE static Time createTime(const Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return obj.createTime((long long)sampleRate, (double)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeUtil2__<TSampleRate, false, TSampleCount, true>
	{
		HX_INLINE static void updateTime(Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			obj.updateTime((double)sampleRate, (long long)sampleCount);
		}
		HX_INLINE static Time createTime(const Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return obj.createTime((double)sampleRate, (long long)sampleCount);
		}
	};
	template <typename TSampleRate, typename TSampleCount>
	struct __TimeUtil2__<TSampleRate, false, TSampleCount, false>
	{
		HX_INLINE static void updateTime(Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			obj.updateTime((double)sampleRate, (double)sampleCount);
		}
		HX_INLINE static Time createTime(const Time& obj, TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
		{
			return obj.createTime((double)sampleRate, (double)sampleCount);
		}
	};

	template <typename TSampleRate, typename TSampleCount>
	HX_INLINE void Time::updateTime(TSampleRate sampleRate, TSampleCount sampleCount) HX_NOEXCEPT
	{
		__TimeUtil2__<TSampleRate, ::std::is_integral<TSampleRate>::value, TSampleCount, ::std::is_integral<TSampleCount>::value> ::updateTime(*this, sampleRate, sampleCount);
	}
	template <typename TSampleRate, typename TSampleCount>
	HX_INLINE Time Time::createTime(TSampleRate sampleRate, TSampleCount sampleCount) const HX_NOEXCEPT
	{
		return __TimeUtil2__<TSampleRate, ::std::is_integral<TSampleRate>::value, TSampleCount, ::std::is_integral<TSampleCount>::value> ::createTime(*this, sampleRate, sampleCount);
	}
}
using HxTime = ::HxLibs::Time;
namespace std
{
	//定义标准库哈希
	template <>
	struct hash<HxTime> : public HxTime::Hash
	{
		using result_type = size_t;
		using argument_type = HxTime;
	};
}
#else //!__cplusplus
#include <time.h>
typedef struct HxTime
{
	union
	{
		time_t secs;
		time_t seconds;
	};
	union
	{
		long long nanos;
		long long nanoseconds;
	};
}HxTime;
#endif //!__cplusplus

#endif //!HX_TIME

#include "BufView.h"

#ifndef HX_TIMES
#define HX_TIMES

HX_BUFVIEW_DEFINE_ROOT_NS(Time)

#endif //HX_TIMES

#endif //!HX_CORE_TIME_H
