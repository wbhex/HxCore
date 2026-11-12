/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_FRACTION_H
#define HX_CORE_FRACTION_H

#ifndef HX_FRACTION
#define HX_FRACTION

#ifdef __cplusplus

#include <assert.h>
#include <stdexcept>
#include "GcdFunc.h"
#include "IntTypes.h"

namespace HxLibs
{
	//表示分数辅助类
	template <typename T, bool = IntUtils<T>::Signed>
	struct __FractionUtils__;
	//表示分数辅助类，有符号整数
	template <typename T>
	struct __FractionUtils__<T, true>
	{
		//表示宽类型
		using WiderType = typename IntUtils<T>::WiderType;
		//化简操作
		HX_CONSTEXPR17_FUNC static void simplify(T& num, T& den) HX_NOEXCEPT
		{
			if (num == 0 || den == 0)
			{
				num = 0;
				den = 1;
			}
			else if (den < 0)
			{//保证分母始终为正值
				num = -num;
				den = -den;
			}
			if (den != 1)
			{
				auto gcd = __gcd__(num < 0 ? -num : num, den);
				if (gcd != 1)
				{
					num /= gcd;
					den /= gcd;
				}
			}
		}
	};
	//表示分数辅助类，无符号整数
	template <typename T>
	struct __FractionUtils__<T, false>
	{
		//表示宽类型
		using WiderType = typename IntUtils<T>::WiderType;
		//化简操作
		HX_CONSTEXPR17_FUNC static void simplify(T& num, T& den) HX_NOEXCEPT
		{
			if (num == 0 || den == 0)
			{
				num = 0;
				den = 1;
			}
			if (den != 1)
			{
				auto gcd = __gcd__(num, den);
				if (gcd != 1)
				{
					num /= gcd;
					den /= gcd;
				}
			}
		}
	};

	//表示分数值类型
	template <typename T>
	struct __FractionValue__
	{
		//表示数据
		struct Data
		{
			//分子值
			T num;
			//分母值
			T den;
		};
		//内部数据
		union
		{
			struct
			{
				//分子值
				T num;
				//分母值
				T den;
			};
			//数据内容
			Data data;
		};

		//表示简化标记
		struct Simplifying
		{
			//NOTHING
		};

		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionValue__(const __FractionValue__<T>& v) HX_NOEXCEPT
			: num(v.num), den(v.den)
		{
			//NOTHING
		}
		//构造函数。
		template <typename T2>
		HX_CONSTEXPR11_FUNC __FractionValue__(const __FractionValue__<T2>& v) HX_NOEXCEPT
			: num((T)v.num), den((T)v.den)
		{
			//NOTHING
		}

		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(T num, T den) HX_NOEXCEPT
			: num(num), den(den)
		{
			assert(den != 0);
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(T num, T den, Simplifying) HX_NOEXCEPT
			: num(num), den(den)
		{
			using Utils = __FractionUtils__<T, IntUtils<T>::Signed>;
			Utils::simplify(data.num, data.den);
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(T num, T den, bool simplifying) HX_NOEXCEPT
			: num(num), den(den)
		{
			if (simplifying)
			{
				using Utils = __FractionUtils__<T, IntUtils<T>::Signed>;
				Utils::simplify(data.num, data.den);
			}
			else
			{
				assert(den != 0);
			}
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionValue__(T num, const __FractionValue__& den) HX_NOEXCEPT
			: __FractionValue__(num* den.den, den.num)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(T num, const __FractionValue__& den, Simplifying simplifying) HX_NOEXCEPT
			: __FractionValue__(num* den.den, den.num, simplifying)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(T num, const __FractionValue__& den, bool simplifying) HX_NOEXCEPT
			: __FractionValue__(num* den.den, den.num, simplifying)
		{
			//NOTHING
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionValue__(const __FractionValue__& num, T den) HX_NOEXCEPT
			: __FractionValue__(num.num, num.den* den)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(const __FractionValue__& num, T den, Simplifying simplifying) HX_NOEXCEPT
			: __FractionValue__(num.num, num.den* den, simplifying)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(const __FractionValue__& num, T den, bool simplifying) HX_NOEXCEPT
			: __FractionValue__(num.num, num.den* den, simplifying)
		{
			//NOTHING
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionValue__(const __FractionValue__& num, const __FractionValue__& den) HX_NOEXCEPT
			: __FractionValue__(num.num* den.den, num.den* den.num)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(const __FractionValue__& num, const __FractionValue__& den, Simplifying simplifying) HX_NOEXCEPT
			: __FractionValue__(num.num* den.den, num.den* den.num, simplifying)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionValue__(const __FractionValue__& num, const __FractionValue__& den, bool simplifying) HX_NOEXCEPT
			: __FractionValue__(num.num* den.den, num.den* den.num, simplifying)
		{
			//NOTHING
		}

		//执行比较操作: 1, a > b; 0, a == b; -1, a < b
		HX_CONSTEXPR17_FUNC static int compare(const __FractionValue__& a, T b) HX_NOEXCEPT
		{
			auto tmp = a.den * b;
			return a.num > tmp ? 1 : (a.num == tmp) ? 0 : -1;
		}
		//执行比较操作: 1, a > b; 0, a == b; -1, a < b
		HX_CONSTEXPR17_FUNC static int compare(T a, const __FractionValue__& b) HX_NOEXCEPT
		{
			auto tmp = a * b.den;
			return tmp > b.num ? 1 : (tmp == b.num) ? 0 : -1;
		}
		//执行比较操作: 1, a > b; 0, a == b; -1, a < b
		HX_CONSTEXPR17_FUNC static int compare(const __FractionValue__& a, const __FractionValue__& b) HX_NOEXCEPT
		{
			auto gcd = ::HxLibs::gcd(a.den, b.den);
			T left, right;
			if (gcd == 1)
			{
				left = a.num * b.den;
				right = b.num * a.den;
			}
			else
			{
				left = a.num * (b.den / gcd);
				right = b.num * (a.den / gcd);
			}
			return left > right ? 1 : (left == right) ? 0 : -1;
		}

		//加法操作
		HX_INLINE static __FractionValue__ add(const __FractionValue__& a, T b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num + b * a.den, a.den);
		}
		//加法操作
		HX_INLINE static __FractionValue__ add(T a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den + b.num, b.den);
		}
		//加法操作
		HX_INLINE static __FractionValue__ add(const __FractionValue__& a, const __FractionValue__& b) HX_NOEXCEPT
		{
			if (a.den == b.den)
			{
				return __FractionValue__(a.num + b.num, a.den);
			}
			auto gcd = ::HxLibs::gcd(a.den, b.den);
			if (gcd == 1)
			{
				return __FractionValue__(b.den * a.num + a.den * b.num, a.den * b.den);
			}
			else
			{
				return __FractionValue__((b.den / gcd) * a.num + (a.den / gcd) * b.num, (a.den / gcd) * b.den);
			}
		}

		//减法操作
		HX_INLINE static __FractionValue__ subtract(const __FractionValue__& a, T b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num - b * a.den, a.den);
		}
		//减法操作
		HX_INLINE static __FractionValue__ subtract(T a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den - b.num, b.den);
		}
		//减法操作
		HX_INLINE static __FractionValue__ subtract(const __FractionValue__& a, const __FractionValue__& b) HX_NOEXCEPT
		{
			if (a.den == b.den)
			{
				return __FractionValue__(a.num - b.num, a.den);
			}
			auto gcd = ::HxLibs::gcd(a.den, b.den);
			if (gcd == 1)
			{
				return __FractionValue__(b.den * a.num - a.den * b.num, a.den * b.den);
			}
			else
			{
				return __FractionValue__((b.den / gcd) * a.num - (a.den / gcd) * b.num, (a.den / gcd) * b.den);
			}
		}

		//乘法操作
		HX_INLINE static __FractionValue__ multiply(const __FractionValue__& a, T b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b, a.den);
		}
		//乘法操作
		HX_INLINE static __FractionValue__ multiply(T a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.num, b.den);
		}
		//乘法操作
		HX_INLINE static __FractionValue__ multiply(const __FractionValue__& a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b.num, a.den * b.den);
		}

		//除法操作
		HX_INLINE static __FractionValue__ divide(const __FractionValue__& a, T b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num, a.den * b);
		}
		//除法操作
		HX_INLINE static __FractionValue__ divide(T a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den, b.num);
		}
		//除法操作
		HX_INLINE static __FractionValue__ divide(const __FractionValue__& a, const __FractionValue__& b) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b.den, a.den * b.num);
		}

		//加法操作
		HX_INLINE static __FractionValue__ add(const __FractionValue__& a, T b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num + b * a.den, a.den, simplifying);
		}
		//加法操作
		HX_INLINE static __FractionValue__ add(T a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den + b.num, b.den, simplifying);
		}
		//加法操作
		HX_INLINE static __FractionValue__ add(const __FractionValue__& a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			if (a.den == b.den)
			{
				return __FractionValue__(a.num + b.num, a.den, simplifying);
			}
			auto gcd = ::HxLibs::gcd(a.den, b.den);
			if (gcd == 1)
			{
				return __FractionValue__(b.den * a.num + a.den * b.num, a.den * b.den, simplifying);
			}
			else
			{
				return __FractionValue__((b.den / gcd) * a.num + (a.den / gcd) * b.num, (a.den / gcd) * b.den, simplifying);
			}
		}

		//减法操作
		HX_INLINE static __FractionValue__ subtract(const __FractionValue__& a, T b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num - b * a.den, a.den, simplifying);
		}
		//减法操作
		HX_INLINE static __FractionValue__ subtract(T a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den - b.num, b.den, simplifying);
		}
		//减法操作
		HX_INLINE static __FractionValue__ subtract(const __FractionValue__& a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			if (a.den == b.den)
			{
				return __FractionValue__(a.num - b.num, a.den, simplifying);
			}
			auto gcd = ::HxLibs::gcd(a.den, b.den);
			if (gcd == 1)
			{
				return __FractionValue__(b.den * a.num - a.den * b.num, a.den * b.den, simplifying);
			}
			else
			{
				return __FractionValue__((b.den / gcd) * a.num - (a.den / gcd) * b.num, (a.den / gcd) * b.den, simplifying);
			}
		}

		//乘法操作
		HX_INLINE static __FractionValue__ multiply(const __FractionValue__& a, T b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b, a.den, simplifying);
		}
		//乘法操作
		HX_INLINE static __FractionValue__ multiply(T a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.num, b.den, simplifying);
		}
		//乘法操作
		HX_INLINE static __FractionValue__ multiply(const __FractionValue__& a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b.num, a.den * b.den, simplifying);
		}

		//除法操作
		HX_INLINE static __FractionValue__ divide(const __FractionValue__& a, T b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num, a.den * b, simplifying);
		}
		//除法操作
		HX_INLINE static __FractionValue__ divide(T a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a * b.den, b.num, simplifying);
		}
		//除法操作
		HX_INLINE static __FractionValue__ divide(const __FractionValue__& a, const __FractionValue__& b, Simplifying simplifying) HX_NOEXCEPT
		{
			return __FractionValue__(a.num * b.den, a.den * b.num, simplifying);
		}
	};

	//表示分数公共基类
	template <typename T>
	struct __FractionBase__
	{
	public:
		//内部值
		using Value = typename __FractionValue__<T>::Data;
		//表示简化标记
		using Simplifying = typename __FractionValue__<T>::Simplifying;

	protected:
		//内部数据
		union
		{
			struct
			{
				//分子值
				T m_num;
				//分母值
				T m_den;
			};
			//整体值
			__FractionValue__<T> m_value;
		};

		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionBase__() HX_NOEXCEPT
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC __FractionBase__(const __FractionValue__<T>& value) HX_NOEXCEPT
			: m_value(value)
		{
			//NOTHING
		}

	public:
		//获取内部值。
		HX_CONSTEXPR17_FUNC Value& value() HX_NOEXCEPT
		{
			return m_value.data;
		}
		//获取内部值。
		HX_CONSTEXPR17_FUNC const Value& value() const HX_NOEXCEPT
		{
			return m_value.data;
		}

		//获取分子。
		HX_CONSTEXPR11_FUNC T num() const HX_NOEXCEPT
		{
			return m_num;
		}
		//获取分子。
		HX_CONSTEXPR11_FUNC T numerator() const HX_NOEXCEPT
		{
			return m_num;
		}

		//获取分母。
		HX_CONSTEXPR11_FUNC T den() const HX_NOEXCEPT
		{
			return m_den;
		}
		//获取分母。
		HX_CONSTEXPR11_FUNC T denominator() const HX_NOEXCEPT
		{
			return m_den;
		}

		//转换为极简表示（除以最大公约数）
		HX_CONSTEXPR17_FUNC void simplify() HX_NOEXCEPT
		{
			using Utils = __FractionUtils__<T, IntUtils<T>::Signed>;
			Utils::simplify(m_num, m_den);
		}

		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionBase__(T num, T den) HX_NOEXCEPT
			: m_value(num, den)
		{
			assert(den != 0);
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionBase__(T num, T den, Simplifying) HX_NOEXCEPT
			: m_value(num, den)
		{
			using Utils = __FractionUtils__<T, IntUtils<T>::Signed>;
			Utils::simplify(m_num, m_den);
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __FractionBase__(T num, T den, bool simplifying) HX_NOEXCEPT
			: m_value(num, den)
		{
			if (simplifying)
			{
				using Utils = __FractionUtils__<T, IntUtils<T>::Signed>;
				Utils::simplify(m_num, m_den);
			}
			else
			{
				assert(den != 0);
			}
		}

		//重置为指定值。
		HX_INLINE void reset(T num) HX_NOEXCEPT
		{
			m_num = num;
			m_den = 1;
		}
		//重置为指定值。
		HX_INLINE void reset(const __FractionBase__& b) HX_NOEXCEPT
		{
			m_num = b.m_num;
			m_den = b.m_den;
		}

		//重置为指定值。
		HX_INLINE void reset(T num, T den) HX_NOEXCEPT
		{
			assert(den != 0);
			m_num = num;
			m_den = den;
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, Simplifying) HX_NOEXCEPT
		{
			m_num = num;
			m_den = den;
			simplify();
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, bool simplifying) HX_NOEXCEPT
		{
			m_num = num;
			m_den = den;
			if (simplifying)
			{
				simplify();
			}
			else
			{
				assert(den != 0);
			}
		}

		//返回单精度浮点数表示。
		HX_CONSTEXPR11_FUNC float toSingle() const HX_NOEXCEPT
		{
			return (float)((double)m_num / (double)m_den);
		}
		//返回双精度浮点数表示。
		HX_CONSTEXPR11_FUNC double toDouble() const HX_NOEXCEPT
		{
			return (double)m_num / (double)m_den;
		}

		//OPERATOR double
		HX_CONSTEXPR11_FUNC operator double() const HX_NOEXCEPT
		{
			return toDouble();
		}
	};

	//表示分数的具体类型
	template <typename T, bool = ::std::is_same<T, typename IntUtils<T>::WiderType>::value>
	struct __Fraction__;
	//表示分数的具体类型，运算中类型和宽类型相同
	template <typename T>
	struct __Fraction__<T, true> : public __FractionBase__<T>
	{
		HX_STATIC_ASSERT(IntUtils<T>::Valid, "HxLibs::Fraction<T> Type parameter must be integral.");

	private:
		//基类型
		using _MyBase = __FractionBase__<T>;
		//临时值
		using TempValue = __FractionValue__<T>;

		//重置为指定值。
		HX_INLINE void reset(const TempValue& value) HX_NOEXCEPT
		{
			_MyBase::m_value = value;
		}
		//构造函数。
		HX_INLINE __Fraction__(const TempValue& value) HX_NOEXCEPT
			: _MyBase(value)
		{
			//NOTHING
		}

	public:
		using Simplifying = typename __FractionBase__<T>::Simplifying;

		//重置为指定值。
		HX_INLINE void reset(T num) HX_NOEXCEPT
		{
			_MyBase::reset(num);
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& b) HX_NOEXCEPT
		{
			_MyBase::reset(b);
		}

		//重置为指定值。
		HX_INLINE void reset(T num, T den) HX_NOEXCEPT
		{
			_MyBase::reset(num, den);
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, Simplifying simplifying) HX_NOEXCEPT
		{
			_MyBase::reset(num, den, simplifying);
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, bool simplifying) HX_NOEXCEPT
		{
			_MyBase::reset(num, den, simplifying);
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__() HX_NOEXCEPT
			: _MyBase(0, 1)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(T num) HX_NOEXCEPT
			: _MyBase(num, 1)
		{
			//NOTHING
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(T num, T den) HX_NOEXCEPT
			: _MyBase(num, den)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __Fraction__(T num, T den, Simplifying simplifying) HX_NOEXCEPT
			: _MyBase(num, den, simplifying)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __Fraction__(T num, T den, bool simplifying) HX_NOEXCEPT
			: _MyBase(num, den, simplifying)
		{
			//NOTHING
		}

		//复制构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(const __Fraction__& other) HX_NOEXCEPT
			: _MyBase(other.m_value)
		{
			//NOTHING
		}

		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den) HX_NOEXCEPT
		{
			reset(TempValue(num, den.m_value));
		}
		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num, den.m_value, simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den, bool simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num, den.m_value, simplifying));
		}

		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den, simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den, bool simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den, simplifying));
		}

		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den.m_value));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den.m_value, simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den, bool simplifying) HX_NOEXCEPT
		{
			reset(TempValue(num.m_value, den.m_value, simplifying));
		}

		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den) HX_NOEXCEPT
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den, bool simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}

		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den) HX_NOEXCEPT
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den, bool simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}

		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den) HX_NOEXCEPT
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den, Simplifying simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den, bool simplifying) HX_NOEXCEPT
		{
			reset(num, den, simplifying);
		}

#pragma region Static Operators

		//等于比较符
		HX_CONSTEXPR11_FUNC static bool equal(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return TempValue::compare(a.m_value, b) == 0;
		}
		//等于比较符
		HX_CONSTEXPR11_FUNC static bool equal(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return TempValue::compare(a, b.m_value) == 0;
		}
		//等于比较符
		HX_CONSTEXPR17_FUNC static bool equal(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num == b.m_num;
			}
			return TempValue::compare(a.m_value, b.m_value) == 0;
		}

		//小于比较符
		HX_CONSTEXPR11_FUNC static bool less(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return TempValue::compare(a.m_value, b) < 0;
		}
		//小于比较符
		HX_CONSTEXPR11_FUNC static bool less(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return TempValue::compare(a, b.m_value) < 0;
		}
		//小于比较符
		HX_CONSTEXPR17_FUNC static bool less(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num < b.m_num;
			}
			return TempValue::compare(a.m_value, b.m_value) < 0;
		}

		//小于等于比较符
		HX_CONSTEXPR11_FUNC static bool lessEqual(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return TempValue::compare(a.m_value, b) <= 0;
		}
		//小于等于比较符
		HX_CONSTEXPR11_FUNC static bool lessEqual(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return TempValue::compare(a, b.m_value) <= 0;
		}
		//小于等于比较符
		HX_CONSTEXPR17_FUNC static bool lessEqual(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num <= b.m_num;
			}
			return TempValue::compare(a.m_value, b.m_value) <= 0;
		}

		//大于比较符
		HX_INLINE static bool greater(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return TempValue::compare(a.m_value, b) > 0;
		}
		//大于比较符
		HX_INLINE static bool greater(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return TempValue::compare(a, b.m_value) > 0;
		}
		//大于比较符
		HX_INLINE static bool greater(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num > b.m_num;
			}
			return TempValue::compare(a.m_value, b.m_value) > 0;
		}

		//大于等于比较符
		HX_INLINE static bool greaterEqual(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return TempValue::compare(a.m_value, b) >= 0;
		}
		//大于等于比较符
		HX_INLINE static bool greaterEqual(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return TempValue::compare(a, b.m_value) >= 0;
		}
		//大于等于比较符
		HX_INLINE static bool greaterEqual(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num >= b.m_num;
			}
			return TempValue::compare(a.m_value, b.m_value) >= 0;
		}

		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			auto t = TempValue::add(a.m_value, b, Simplifying{});
			return __Fraction__(t);
		}
		//加法操作
		HX_INLINE static __Fraction__ add(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::add(a, b.m_value, Simplifying{});
			return __Fraction__(t);
		}
		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::add(a.m_value, b.m_value, Simplifying{});
			return __Fraction__(t);
		}

		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			auto t = TempValue::subtract(a.m_value, b, Simplifying{});
			return __Fraction__(t);
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::subtract(a, b.m_value, Simplifying{});
			return __Fraction__(t);
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::subtract(a.m_value, b.m_value, Simplifying{});
			return __Fraction__(t);
		}

		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			auto t = TempValue::multiply(a.m_value, b, Simplifying{});
			return __Fraction__(t);
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::multiply(a, b.m_value, Simplifying{});
			return __Fraction__(t);
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::multiply(a.m_value, b.m_value, Simplifying{});
			return __Fraction__(t);
		}

		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			auto t = TempValue::divide(a.m_value, b, Simplifying{});
			return __Fraction__(t);
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::divide(a, b.m_value, Simplifying{});
			return __Fraction__(t);
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::divide(a.m_value, b.m_value, Simplifying{});
			return __Fraction__(t);
		}

		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, T b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::add(a.m_value, b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::add(a.m_value, b);
				return __Fraction__(t);
			}
		}
		//加法操作
		HX_INLINE static __Fraction__ add(T a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::add(a, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::add(a, b.m_value);
				return __Fraction__(t);
			}
		}
		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::add(a.m_value, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::add(a.m_value, b.m_value);
				return __Fraction__(t);
			}
		}

		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, T b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::subtract(a.m_value, b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::subtract(a.m_value, b);
				return __Fraction__(t);
			}
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(T a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::subtract(a, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::subtract(a, b.m_value);
				return __Fraction__(t);
			}
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::subtract(a.m_value, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::subtract(a.m_value, b.m_value);
				return __Fraction__(t);
			}
		}

		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, T b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::multiply(a.m_value, b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::multiply(a.m_value, b);
				return __Fraction__(t);
			}
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(T a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::multiply(a, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::multiply(a, b.m_value);
				return __Fraction__(t);
			}
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::multiply(a.m_value, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::multiply(a.m_value, b.m_value);
				return __Fraction__(t);
			}
		}

		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, T b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::divide(a.m_value, b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::divide(a.m_value, b);
				return __Fraction__(t);
			}
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(T a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::divide(a, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::divide(a, b.m_value);
				return __Fraction__(t);
			}
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, const __Fraction__& b, bool simplifying) HX_NOEXCEPT
		{
			if (simplifying)
			{
				auto t = TempValue::divide(a.m_value, b.m_value, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = TempValue::divide(a.m_value, b.m_value);
				return __Fraction__(t);
			}
		}

#pragma endregion //Static Operators

		//等于比较符
		HX_INLINE bool equal(T b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool equal(const __Fraction__& b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}

		//小于比较符
		HX_INLINE bool less(T b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}
		//小于比较符
		HX_INLINE bool less(const __Fraction__& b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}

		//小于等于比较符
		HX_INLINE bool lessEqual(T b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}
		//小于等于比较符
		HX_INLINE bool lessEqual(const __Fraction__& b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}

		//大于比较符
		HX_INLINE bool greater(T b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}
		//大于比较符
		HX_INLINE bool greater(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}

		//大于等于比较符
		HX_INLINE bool greaterEqual(T b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}
		//大于等于比较符
		HX_INLINE bool greaterEqual(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__ add(T b) const HX_NOEXCEPT
		{
			return add(*this, b);
		}
		//加法操作
		HX_INLINE __Fraction__ add(const __Fraction__& b) const HX_NOEXCEPT
		{
			return add(*this, b);
		}

		//减法操作
		HX_INLINE __Fraction__ subtract(T b) const HX_NOEXCEPT
		{
			return subtract(*this, b);
		}
		//减法操作
		HX_INLINE __Fraction__ subtract(const __Fraction__& b) const HX_NOEXCEPT
		{
			return subtract(*this, b);
		}

		//乘法操作
		HX_INLINE __Fraction__ multiply(T b) const HX_NOEXCEPT
		{
			return multiply(*this, b);
		}
		//乘法操作
		HX_INLINE __Fraction__ multiply(const __Fraction__& b) const HX_NOEXCEPT
		{
			return multiply(*this, b);
		}

		//除法操作
		HX_INLINE __Fraction__ divide(T b) const HX_NOEXCEPT
		{
			return divide(*this, b);
		}
		//除法操作
		HX_INLINE __Fraction__ divide(const __Fraction__& b) const HX_NOEXCEPT
		{
			return divide(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__ add(T b, bool simplifying) const HX_NOEXCEPT
		{
			return add(*this, b, simplifying);
		}
		//加法操作
		HX_INLINE __Fraction__ add(const __Fraction__& b, bool simplifying) const HX_NOEXCEPT
		{
			return add(*this, b, simplifying);
		}

		//减法操作
		HX_INLINE __Fraction__ subtract(T b, bool simplifying) const HX_NOEXCEPT
		{
			return subtract(*this, b, simplifying);
		}
		//减法操作
		HX_INLINE __Fraction__ subtract(const __Fraction__& b, bool simplifying) const HX_NOEXCEPT
		{
			return subtract(*this, b, simplifying);
		}

		//乘法操作
		HX_INLINE __Fraction__ multiply(T b, bool simplifying) const HX_NOEXCEPT
		{
			return multiply(*this, b, simplifying);
		}
		//乘法操作
		HX_INLINE __Fraction__ multiply(const __Fraction__& b, bool simplifying) const HX_NOEXCEPT
		{
			return multiply(*this, b, simplifying);
		}

		//除法操作
		HX_INLINE __Fraction__ divide(T b, bool simplifying) const HX_NOEXCEPT
		{
			return divide(*this, b, simplifying);
		}
		//除法操作
		HX_INLINE __Fraction__ divide(const __Fraction__& b, bool simplifying) const HX_NOEXCEPT
		{
			return divide(*this, b, simplifying);
		}

		//加法操作
		HX_INLINE __Fraction__ operator+() const HX_NOEXCEPT
		{
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__ operator-() const HX_NOEXCEPT
		{
			HX_STATIC_ASSERT(IntUtils<T>::Signed, "HxLibs::__Fraction__<T> operator-() Type parameter must be signed.");
			return __Fraction__(-_MyBase::m_num, _MyBase::m_den);
		}

		//加法操作
		HX_INLINE __Fraction__ operator+(T b) const HX_NOEXCEPT
		{
			return add(*this, b);
		}
		//加法操作
		HX_INLINE __Fraction__ operator+(const __Fraction__& b) const HX_NOEXCEPT
		{
			return add(*this, b);
		}

		//减法操作
		HX_INLINE __Fraction__ operator-(T b) const HX_NOEXCEPT
		{
			return subtract(*this, b);
		}
		//减法操作
		HX_INLINE __Fraction__ operator-(const __Fraction__& b) const HX_NOEXCEPT
		{
			return subtract(*this, b);
		}

		//乘法操作
		HX_INLINE __Fraction__ operator*(T b) const HX_NOEXCEPT
		{
			return multiply(*this, b);
		}
		//乘法操作
		HX_INLINE __Fraction__ operator*(const __Fraction__& b) const HX_NOEXCEPT
		{
			return multiply(*this, b);
		}

		//除法操作
		HX_INLINE __Fraction__ operator/(T b) const HX_NOEXCEPT
		{
			return divide(*this, b);
		}
		//除法操作
		HX_INLINE __Fraction__ operator/(const __Fraction__& b) const HX_NOEXCEPT
		{
			return divide(*this, b);
		}

		//等于比较符
		HX_INLINE bool operator==(T b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool operator==(const __Fraction__& b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}

		//等于比较符
		HX_INLINE bool operator!=(T b) const HX_NOEXCEPT
		{
			return !equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool operator!=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return !equal(*this, b);
		}

		//小于比较符
		HX_INLINE bool operator <(T b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}
		//小于比较符
		HX_INLINE bool operator <(const __Fraction__& b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}

		//小于等于比较符
		HX_INLINE bool operator <=(T b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}
		//小于等于比较符
		HX_INLINE bool operator <=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}

		//大于比较符
		HX_INLINE bool operator >(T b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}
		//大于比较符
		HX_INLINE bool operator >(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}

		//大于等于比较符
		HX_INLINE bool operator >=(T b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}
		//大于等于比较符
		HX_INLINE bool operator >=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__& operator+=(T b) HX_NOEXCEPT
		{
			auto t = TempValue::add(_MyBase::m_value, b, Simplifying{});
			reset(t);
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__& operator-=(T b) HX_NOEXCEPT
		{
			auto t = TempValue::subtract(_MyBase::m_value, b, Simplifying{});
			reset(t);
			return *this;
		}
		//乘法操作
		HX_INLINE __Fraction__& operator*=(T b) HX_NOEXCEPT
		{
			auto t = TempValue::multiply(_MyBase::m_value, b, Simplifying{});
			reset(t);
			return *this;
		}
		//除法操作
		HX_INLINE __Fraction__& operator/=(T b) HX_NOEXCEPT
		{
			auto t = TempValue::divide(_MyBase::m_value, b, Simplifying{});
			reset(t);
			return *this;
		}

		//加法操作
		HX_INLINE __Fraction__& operator+=(const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::add(_MyBase::m_value, b.m_value, Simplifying{});
			reset(t);
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__& operator-=(const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::subtract(_MyBase::m_value, b.m_value, Simplifying{});
			reset(t);
			return *this;
		}
		//乘法操作
		HX_INLINE __Fraction__& operator*=(const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::multiply(_MyBase::m_value, b.m_value, Simplifying{});
			reset(t);
			return *this;
		}
		//除法操作
		HX_INLINE __Fraction__& operator/=(const __Fraction__& b) HX_NOEXCEPT
		{
			auto t = TempValue::divide(_MyBase::m_value, b.m_value, Simplifying{});
			reset(t);
			return *this;
		}
	};
	//表示分数的具体类型，运算中类型和宽类型不同
	template <typename T>
	struct __Fraction__<T, false> : public __FractionBase__<T>
	{
		HX_STATIC_ASSERT(IntUtils<T>::Valid, "HxLibs::Fraction<T> Type parameter must be integral.");

	private:
		//基类型
		using _MyBase = __FractionBase__<T>;
		//表示更宽类型
		using WiderType = typename IntUtils<T>::WiderType;
		//表示更宽类型值
		using WiderValue = __FractionValue__<WiderType>;

		//重置为指定值。
		HX_INLINE void reset(const WiderValue& value)
		{
			if (value.num < IntUtils<T>::MinValue)
			{
				throw ::std::overflow_error("HxLibs::Fraction<T> numerator is less than min value.");
			}
			if (value.num > IntUtils<T>::MaxValue)
			{
				throw ::std::overflow_error("HxLibs::Fraction<T> numerator is greater than max value.");
			}
			if (value.den < IntUtils<T>::MinValue)
			{
				throw ::std::overflow_error("HxLibs::Fraction<T> denominator is less than min value.");
			}
			if (value.den > IntUtils<T>::MaxValue)
			{
				throw ::std::overflow_error("HxLibs::Fraction<T> denominator is greater than max value.");
			}

			_MyBase::m_num = (T)value.num;
			_MyBase::m_den = (T)value.den;
		}
		//构造函数。
		HX_INLINE __Fraction__(const WiderValue& value) HX_NOEXCEPT
		{
			reset(value);
		}

	public:
		using Simplifying = typename __FractionBase__<T>::Simplifying;

		//重置为指定值。
		HX_INLINE void reset(T num) HX_NOEXCEPT
		{
			_MyBase::reset(num);
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& b) HX_NOEXCEPT
		{
			_MyBase::reset(b);
		}

		//重置为指定值。
		HX_INLINE void reset(T num, T den) HX_NOEXCEPT
		{
			_MyBase::reset(num, den);
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, Simplifying simplifying) HX_NOEXCEPT
		{
			_MyBase::reset(num, den, simplifying);
		}
		//重置为指定值。
		HX_INLINE void reset(T num, T den, bool simplifying) HX_NOEXCEPT
		{
			_MyBase::reset(num, den, simplifying);
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__() HX_NOEXCEPT
			: _MyBase(0, 1)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(T num) HX_NOEXCEPT
			: _MyBase(num, 1)
		{
			//NOTHING
		}

		//构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(T num, T den) HX_NOEXCEPT
			: _MyBase(num, den)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __Fraction__(T num, T den, Simplifying simplifying) HX_NOEXCEPT
			: _MyBase(num, den, simplifying)
		{
			//NOTHING
		}
		//构造函数。
		HX_CONSTEXPR17_FUNC __Fraction__(T num, T den, bool simplifying) HX_NOEXCEPT
			: _MyBase(num, den, simplifying)
		{
			//NOTHING
		}

		//复制构造函数。
		HX_CONSTEXPR11_FUNC __Fraction__(const __Fraction__& other) HX_NOEXCEPT
			: _MyBase(other.m_value)
		{
			//NOTHING
		}

		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den)
		{
			reset(WiderValue(num, WiderValue(den.m_value)));
		}
		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den, Simplifying simplifying)
		{
			reset(WiderValue(num, WiderValue(den.m_value), simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(T num, const __Fraction__& den, bool simplifying)
		{
			reset(WiderValue(num, WiderValue(den.m_value), simplifying));
		}

		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den)
		{
			reset(WiderValue(WiderValue(num.m_value), den));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den, Simplifying simplifying)
		{
			reset(WiderValue(WiderValue(num.m_value), den, simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, T den, bool simplifying)
		{
			reset(WiderValue(WiderValue(num.m_value), den, simplifying));
		}

		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den)
		{
			reset(WiderValue(WiderValue(num.m_value), WiderValue(den.m_value)));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den, Simplifying simplifying)
		{
			reset(WiderValue(WiderValue(num.m_value), WiderValue(den.m_value), simplifying));
		}
		//重置为指定值。
		HX_INLINE void reset(const __Fraction__& num, const __Fraction__& den, bool simplifying)
		{
			reset(WiderValue(WiderValue(num.m_value), WiderValue(den.m_value), simplifying));
		}

		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den)
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den, Simplifying simplifying)
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(T num, const __Fraction__& den, bool simplifying)
		{
			reset(num, den, simplifying);
		}

		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den)
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den, Simplifying simplifying)
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, T den, bool simplifying)
		{
			reset(num, den, simplifying);
		}

		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den)
		{
			reset(num, den);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den, Simplifying simplifying)
		{
			reset(num, den, simplifying);
		}
		//构造函数。
		HX_INLINE __Fraction__(const __Fraction__& num, const __Fraction__& den, bool simplifying)
		{
			reset(num, den, simplifying);
		}

#pragma region Static Operators

		//等于比较符
		HX_CONSTEXPR11_FUNC static bool equal(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return WiderValue::compare(WiderValue(a.m_value), b) == 0;
		}
		//等于比较符
		HX_CONSTEXPR11_FUNC static bool equal(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return WiderValue::compare(a, WiderValue(b.m_value)) == 0;
		}
		//等于比较符
		HX_CONSTEXPR17_FUNC static bool equal(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num == b.m_num;
			}
			return WiderValue::compare(WiderValue(a.m_value), WiderValue(b.m_value)) == 0;
		}

		//小于比较符
		HX_CONSTEXPR11_FUNC static bool less(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return WiderValue::compare(WiderValue(a.m_value), b) < 0;
		}
		//小于比较符
		HX_CONSTEXPR11_FUNC static bool less(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return WiderValue::compare(a, WiderValue(b.m_value)) < 0;
		}
		//小于比较符
		HX_CONSTEXPR17_FUNC static bool less(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num < b.m_num;
			}
			return WiderValue::compare(WiderValue(a.m_value), WiderValue(b.m_value)) < 0;
		}

		//小于等于比较符
		HX_CONSTEXPR11_FUNC static bool lessEqual(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return WiderValue::compare(WiderValue(a.m_value), b) <= 0;
		}
		//小于等于比较符
		HX_CONSTEXPR11_FUNC static bool lessEqual(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return WiderValue::compare(a, WiderValue(b.m_value)) <= 0;
		}
		//小于等于比较符
		HX_CONSTEXPR17_FUNC static bool lessEqual(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num <= b.m_num;
			}
			return WiderValue::compare(WiderValue(a.m_value), WiderValue(b.m_value)) <= 0;
		}

		//大于比较符
		HX_INLINE static bool greater(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return WiderValue::compare(WiderValue(a.m_value), b) > 0;
		}
		//大于比较符
		HX_INLINE static bool greater(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return WiderValue::compare(a, WiderValue(b.m_value)) > 0;
		}
		//大于比较符
		HX_INLINE static bool greater(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num > b.m_num;
			}
			return WiderValue::compare(WiderValue(a.m_value), WiderValue(b.m_value)) > 0;
		}

		//大于等于比较符
		HX_INLINE static bool greaterEqual(const __Fraction__& a, T b) HX_NOEXCEPT
		{
			return WiderValue::compare(WiderValue(a.m_value), b) >= 0;
		}
		//大于等于比较符
		HX_INLINE static bool greaterEqual(T a, const __Fraction__& b) HX_NOEXCEPT
		{
			return WiderValue::compare(a, WiderValue(b.m_value)) >= 0;
		}
		//大于等于比较符
		HX_INLINE static bool greaterEqual(const __Fraction__& a, const __Fraction__& b) HX_NOEXCEPT
		{
			if (a.m_den == b.m_den)
			{
				return a.m_num >= b.m_num;
			}
			return WiderValue::compare(WiderValue(a.m_value), WiderValue(b.m_value)) >= 0;
		}

		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, T b)
		{
			auto t = WiderValue::add(WiderValue(a.m_value), b, Simplifying{});
			return __Fraction__(t);
		}
		//加法操作
		HX_INLINE static __Fraction__ add(T a, const __Fraction__& b)
		{
			auto t = WiderValue::add(a, WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}
		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, const __Fraction__& b)
		{
			auto t = WiderValue::add(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}

		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, T b)
		{
			auto t = WiderValue::subtract(WiderValue(a.m_value), b, Simplifying{});
			return __Fraction__(t);
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(T a, const __Fraction__& b)
		{
			auto t = WiderValue::subtract(a, WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, const __Fraction__& b)
		{
			auto t = WiderValue::subtract(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}

		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, T b)
		{
			auto t = WiderValue::multiply(WiderValue(a.m_value), b, Simplifying{});
			return __Fraction__(t);
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(T a, const __Fraction__& b)
		{
			auto t = WiderValue::multiply(a, WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, const __Fraction__& b)
		{
			auto t = WiderValue::multiply(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}

		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, T b)
		{
			auto t = WiderValue::divide(WiderValue(a.m_value), b, Simplifying{});
			return __Fraction__(t);
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(T a, const __Fraction__& b)
		{
			auto t = WiderValue::divide(a, WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, const __Fraction__& b)
		{
			auto t = WiderValue::divide(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
			return __Fraction__(t);
		}

		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, T b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::add(WiderValue(a.m_value), b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::add(WiderValue(a.m_value), b);
				return __Fraction__(t);
			}
		}
		//加法操作
		HX_INLINE static __Fraction__ add(T a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::add(a, WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::add(a, WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}
		//加法操作
		HX_INLINE static __Fraction__ add(const __Fraction__& a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::add(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::add(WiderValue(a.m_value), WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}

		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, T b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::subtract(WiderValue(a.m_value), b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::subtract(WiderValue(a.m_value), b);
				return __Fraction__(t);
			}
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(T a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::subtract(a, WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::subtract(a, WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}
		//减法操作
		HX_INLINE static __Fraction__ subtract(const __Fraction__& a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::subtract(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::subtract(WiderValue(a.m_value), WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}

		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, T b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::multiply(WiderValue(a.m_value), b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::multiply(WiderValue(a.m_value), b);
				return __Fraction__(t);
			}
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(T a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::multiply(a, WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::multiply(a, WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}
		//乘法操作
		HX_INLINE static __Fraction__ multiply(const __Fraction__& a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::multiply(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::multiply(WiderValue(a.m_value), WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}

		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, T b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::divide(WiderValue(a.m_value), b, Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::divide(WiderValue(a.m_value), b);
				return __Fraction__(t);
			}
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(T a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::divide(a, WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::divide(a, WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}
		//除法操作
		HX_INLINE static __Fraction__ divide(const __Fraction__& a, const __Fraction__& b, bool simplifying)
		{
			if (simplifying)
			{
				auto t = WiderValue::divide(WiderValue(a.m_value), WiderValue(b.m_value), Simplifying{});
				return __Fraction__(t);
			}
			else
			{
				auto t = WiderValue::divide(WiderValue(a.m_value), WiderValue(b.m_value));
				return __Fraction__(t);
			}
		}

#pragma endregion //Static Operators

		//等于比较符
		HX_INLINE bool equal(T b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool equal(const __Fraction__& b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}

		//小于比较符
		HX_INLINE bool less(T b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}
		//小于比较符
		HX_INLINE bool less(const __Fraction__& b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}

		//小于等于比较符
		HX_INLINE bool lessEqual(T b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}
		//小于等于比较符
		HX_INLINE bool lessEqual(const __Fraction__& b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}

		//大于比较符
		HX_INLINE bool greater(T b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}
		//大于比较符
		HX_INLINE bool greater(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}

		//大于等于比较符
		HX_INLINE bool greaterEqual(T b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}
		//大于等于比较符
		HX_INLINE bool greaterEqual(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__ add(T b) const
		{
			return add(*this, b);
		}
		//加法操作
		HX_INLINE __Fraction__ add(const __Fraction__& b) const
		{
			return add(*this, b);
		}

		//减法操作
		HX_INLINE __Fraction__ subtract(T b) const
		{
			return subtract(*this, b);
		}
		//减法操作
		HX_INLINE __Fraction__ subtract(const __Fraction__& b) const
		{
			return subtract(*this, b);
		}

		//乘法操作
		HX_INLINE __Fraction__ multiply(T b) const
		{
			return multiply(*this, b);
		}
		//乘法操作
		HX_INLINE __Fraction__ multiply(const __Fraction__& b) const
		{
			return multiply(*this, b);
		}

		//除法操作
		HX_INLINE __Fraction__ divide(T b) const
		{
			return divide(*this, b);
		}
		//除法操作
		HX_INLINE __Fraction__ divide(const __Fraction__& b) const
		{
			return divide(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__ add(T b, bool simplifying) const
		{
			return add(*this, b, simplifying);
		}
		//加法操作
		HX_INLINE __Fraction__ add(const __Fraction__& b, bool simplifying) const
		{
			return add(*this, b, simplifying);
		}

		//减法操作
		HX_INLINE __Fraction__ subtract(T b, bool simplifying) const
		{
			return subtract(*this, b, simplifying);
		}
		//减法操作
		HX_INLINE __Fraction__ subtract(const __Fraction__& b, bool simplifying) const
		{
			return subtract(*this, b, simplifying);
		}

		//乘法操作
		HX_INLINE __Fraction__ multiply(T b, bool simplifying) const
		{
			return multiply(*this, b, simplifying);
		}
		//乘法操作
		HX_INLINE __Fraction__ multiply(const __Fraction__& b, bool simplifying) const
		{
			return multiply(*this, b, simplifying);
		}

		//除法操作
		HX_INLINE __Fraction__ divide(T b, bool simplifying) const
		{
			return divide(*this, b, simplifying);
		}
		//除法操作
		HX_INLINE __Fraction__ divide(const __Fraction__& b, bool simplifying) const
		{
			return divide(*this, b, simplifying);
		}

		//加法操作
		HX_INLINE __Fraction__ operator+() const HX_NOEXCEPT
		{
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__ operator-() const HX_NOEXCEPT
		{
			HX_STATIC_ASSERT(IntUtils<T>::Signed, "HxLibs::__Fraction__<T> operator-() Type parameter must be signed.");
			return __Fraction__(-_MyBase::m_num, _MyBase::m_den);
		}

		//加法操作
		HX_INLINE __Fraction__ operator+(T b) const
		{
			return add(*this, b);
		}
		//加法操作
		HX_INLINE __Fraction__ operator+(const __Fraction__& b) const
		{
			return add(*this, b);
		}

		//减法操作
		HX_INLINE __Fraction__ operator-(T b) const
		{
			return subtract(*this, b);
		}
		//减法操作
		HX_INLINE __Fraction__ operator-(const __Fraction__& b) const
		{
			return subtract(*this, b);
		}

		//乘法操作
		HX_INLINE __Fraction__ operator*(T b) const
		{
			return multiply(*this, b);
		}
		//乘法操作
		HX_INLINE __Fraction__ operator*(const __Fraction__& b) const
		{
			return multiply(*this, b);
		}

		//除法操作
		HX_INLINE __Fraction__ operator/(T b) const
		{
			return divide(*this, b);
		}
		//除法操作
		HX_INLINE __Fraction__ operator/(const __Fraction__& b) const
		{
			return divide(*this, b);
		}

		//等于比较符
		HX_INLINE bool operator==(T b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool operator==(const __Fraction__& b) const HX_NOEXCEPT
		{
			return equal(*this, b);
		}

		//等于比较符
		HX_INLINE bool operator!=(T b) const HX_NOEXCEPT
		{
			return !equal(*this, b);
		}
		//等于比较符
		HX_INLINE bool operator!=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return !equal(*this, b);
		}

		//小于比较符
		HX_INLINE bool operator <(T b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}
		//小于比较符
		HX_INLINE bool operator <(const __Fraction__& b) const HX_NOEXCEPT
		{
			return less(*this, b);
		}

		//小于等于比较符
		HX_INLINE bool operator <=(T b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}
		//小于等于比较符
		HX_INLINE bool operator <=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return lessEqual(*this, b);
		}

		//大于比较符
		HX_INLINE bool operator >(T b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}
		//大于比较符
		HX_INLINE bool operator >(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greater(*this, b);
		}

		//大于等于比较符
		HX_INLINE bool operator >=(T b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}
		//大于等于比较符
		HX_INLINE bool operator >=(const __Fraction__& b) const HX_NOEXCEPT
		{
			return greaterEqual(*this, b);
		}

		//加法操作
		HX_INLINE __Fraction__& operator+=(T b)
		{
			auto t = WiderValue::add(WiderValue(_MyBase::m_value), b, Simplifying{});
			reset(t);
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__& operator-=(T b)
		{
			auto t = WiderValue::subtract(WiderValue(_MyBase::m_value), b, Simplifying{});
			reset(t);
			return *this;
		}
		//乘法操作
		HX_INLINE __Fraction__& operator*=(T b)
		{
			auto t = WiderValue::multiply(WiderValue(_MyBase::m_value), b, Simplifying{});
			reset(t);
			return *this;
		}
		//除法操作
		HX_INLINE __Fraction__& operator/=(T b)
		{
			auto t = WiderValue::divide(WiderValue(_MyBase::m_value), b, Simplifying{});
			reset(t);
			return *this;
		}

		//加法操作
		HX_INLINE __Fraction__& operator+=(const __Fraction__& b)
		{
			auto t = WiderValue::add(WiderValue(_MyBase::m_value), WiderValue(b.m_value), Simplifying{});
			reset(t);
			return *this;
		}
		//减法操作
		HX_INLINE __Fraction__& operator-=(const __Fraction__& b)
		{
			auto t = WiderValue::subtract(WiderValue(_MyBase::m_value), WiderValue(b.m_value), Simplifying{});
			reset(t);
			return *this;
		}
		//乘法操作
		HX_INLINE __Fraction__& operator*=(const __Fraction__& b)
		{
			auto t = WiderValue::multiply(WiderValue(_MyBase::m_value), WiderValue(b.m_value), Simplifying{});
			reset(t);
			return *this;
		}
		//除法操作
		HX_INLINE __Fraction__& operator/=(const __Fraction__& b)
		{
			auto t = WiderValue::divide(WiderValue(_MyBase::m_value), WiderValue(b.m_value), Simplifying{});
			reset(t);
			return *this;
		}
	};

	//定义分数类型
	template <typename T>
	using Fraction = __Fraction__<T, ::std::is_same<T, typename IntUtils<T>::WiderType>::value>;

	//等于比较符
	template <typename T>
	HX_INLINE bool operator==(T a, const Fraction<T>& b) HX_NOEXCEPT
	{
		return Fraction<T>::equal(a, b);
	}
	//不等于比较符
	template <typename T>
	HX_INLINE bool operator!=(T a, const  Fraction<T>& b) HX_NOEXCEPT
	{
		return !Fraction<T>::equal(a, b);
	}
	//小于比较符
	template <typename T>
	HX_INLINE bool operator <(T a, const  Fraction<T>& b) HX_NOEXCEPT
	{
		return Fraction<T>::less(a, b);
	}
	//小于等于比较符
	template <typename T>
	HX_INLINE bool operator <=(T a, const Fraction<T>& b) HX_NOEXCEPT
	{
		return Fraction<T>::lessEqual(a, b);
	}
	//大于比较符
	template <typename T>
	HX_INLINE bool operator >(T a, const Fraction<T>& b) HX_NOEXCEPT
	{
		return Fraction<T>::greater(a, b);
	}
	//大于等于比较符
	template <typename T>
	HX_INLINE bool operator >=(T a, const Fraction<T>& b) HX_NOEXCEPT
	{
		return Fraction<T>::greaterEqual(a, b);
	}

	//加法操作
	template <typename T>
	HX_INLINE Fraction<T> operator+(T a, const Fraction<T>& b)
	{
		return Fraction<T>::add(a, b);
	}
	//减法操作
	template <typename T>
	HX_INLINE Fraction<T> operator-(T a, const Fraction<T>& b)
	{
		return Fraction<T>::subtract(a, b);
	}
	//乘法操作
	template <typename T>
	HX_INLINE Fraction<T> operator*(T a, const Fraction<T>& b)
	{
		return Fraction<T>::multiply(a, b);
	}
	//除法操作
	template <typename T>
	HX_INLINE Fraction<T> operator/(T a, const Fraction<T>& b)
	{
		return Fraction<T>::divide(a, b);
	}
}

template <typename T>
using HxFraction = ::HxLibs::Fraction<T>;

#define HX_FRACTION_DEFINE(NAME, TYPE) using NAME = ::HxLibs::Fraction<TYPE>;

#else //__cplusplus

#define HX_FRACTION_DEFINE(NAME, TYPE)\
typedef struct NAME\
{\
	TYPE num;\
	TYPE den;\
} NAME;

#endif //__cplusplus

HX_FRACTION_DEFINE(HxFrac8u, Hx8u);
HX_FRACTION_DEFINE(HxFrac8s, Hx8s);
HX_FRACTION_DEFINE(HxFrac16u, Hx16u);
HX_FRACTION_DEFINE(HxFrac16s, Hx16s);
HX_FRACTION_DEFINE(HxFrac32u, Hx32u);
HX_FRACTION_DEFINE(HxFrac32s, Hx32s);
HX_FRACTION_DEFINE(HxFrac64u, Hx64u);
HX_FRACTION_DEFINE(HxFrac64s, Hx64s);

#endif //!HX_FRACTION

#endif //HX_CORE_FRACTION_H
