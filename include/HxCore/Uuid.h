/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_UUID_H
#define HX_CORE_UUID_H

#ifndef HX_UUID
#define HX_UUID

#include "Struct.h"
#ifdef __cplusplus
#include <functional>
#include "HashUtils.h"
#endif //__cplusplus

HX_STRUCT_DECLARE(HxUuid)
{
	union
	{
		struct
		{
			int data1;
			short data2;
			short data3;
		};
		struct
		{
			unsigned int a;
			unsigned short b;
			unsigned short c;
		};
		long long firstData;
	};
	union
	{
		unsigned char data4[8];
		struct
		{
			unsigned char d;
			unsigned char e;
			unsigned char f;
			unsigned char g;
			unsigned char h;
			unsigned char i;
			unsigned char j;
			unsigned char k;
		};
		long long secondData;
	};
#ifdef __cplusplus
	//表示零标记
	struct Zero
	{
		//NOTHING
	};

	//初始化全局标识。
	HX_CONSTEXPR11_FUNC HxUuid() HX_NOEXCEPT
		: firstData(0LL), secondData(0LL)
	{
		//NOTHING
	}
	//初始化全局标识。
	HX_CONSTEXPR11_FUNC HxUuid(Zero) HX_NOEXCEPT
		: firstData(0LL), secondData(0LL)
	{
		//NOTHING
	}
	//初始化全局标识。
	HX_CONSTEXPR11_FUNC HxUuid(const HxUuid & other) HX_NOEXCEPT
		: firstData(other.firstData), secondData(other.secondData)
	{
		//NOTHING
	}
	//初始化全局标识。
	HX_CONSTEXPR11_FUNC HxUuid(unsigned int a, unsigned short b, unsigned short c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i, unsigned char j, unsigned char k) HX_NOEXCEPT
		: a(a), b(b), c(c), d(d), e(e), f(f), g(g), h(h), i(i), j(j), k(k)
	{
		//NOTHING
	}

	//置为0.
	HX_INLINE void zero() HX_NOEXCEPT
	{
		this->firstData = 0LL;
		this->secondData = 0LL;
	}

	//直接更改全局标识。
	HX_INLINE void alter(Zero) HX_NOEXCEPT
	{
		zero();
	}
	//更改全局标识。
	HX_INLINE void alter(const HxUuid & other) HX_NOEXCEPT
	{
		this->firstData = other.firstData;
		this->secondData = other.secondData;
	}

	//置为0。
	HX_INLINE HxUuid& operator=(Zero) HX_NOEXCEPT
	{
		zero();
		return *this;
	}
	//设置为指定值班。
	HX_INLINE HxUuid operator=(const HxUuid & other) HX_NOEXCEPT
	{
		if (this != &other)
		{
			alter(other);
		}
		return *this;
	}

	//判断当前全局标识是否为零值。
	HX_CONSTEXPR11_FUNC bool equals(Zero) const HX_NOEXCEPT
	{
		return firstData == 0LL && secondData == 0LL;
	}
	//判断当前全局标识和指定全局标识是否相同。
	HX_CONSTEXPR11_FUNC bool equals(const HxUuid & other) const HX_NOEXCEPT
	{
		return firstData == other.firstData && secondData == other.secondData;
	}

	//更新全局标识，如果发生改变则返回True。
	HX_INLINE bool update(Zero) HX_NOEXCEPT
	{
		bool changed = false;
		if (this->firstData != 0LL)
		{
			changed = true;
			this->firstData = 0LL;
		}
		if (this->secondData != 0LL)
		{
			changed = true;
			this->secondData = 0LL;
		}
		return changed;
	}
	//更新全局标识，如果发生改变则返回True。
	HX_INLINE bool update(const HxUuid & other) HX_NOEXCEPT
	{
		bool changed = false;
		if (this->firstData != other.firstData)
		{
			changed = true;
			this->firstData = other.firstData;
		}
		if (this->secondData != other.secondData)
		{
			changed = true;
			this->secondData = other.secondData;
		}
		return changed;
	}

	//OPERATOR ==
	HX_CONSTEXPR11_FUNC bool operator==(const HxUuid & right) const HX_NOEXCEPT
	{
		return firstData == right.firstData && secondData == right.secondData;
	}
	//OPERATOR !=
	HX_CONSTEXPR11_FUNC bool operator!=(const HxUuid & right) const HX_NOEXCEPT
	{
		return firstData != right.firstData || secondData != right.secondData;
	}
	//OPERATOR < 
	HX_CONSTEXPR11_FUNC bool operator<(const HxUuid & right) const HX_NOEXCEPT
	{
		if (this == &right)
		{
			return false;
		}
		if (firstData < right.firstData)
		{
			return true;
		}
		else if (firstData > right.firstData)
		{
			return false;
		}
		if (secondData < right.secondData)
		{
			return true;
		}
		else if (secondData > right.secondData)
		{
			return false;
		}
		return false;
	}
	//OPERATOR >
	HX_CONSTEXPR11_FUNC bool operator>(const HxUuid & right) const HX_NOEXCEPT
	{
		if (this == &right)
		{
			return false;
		}
		if (firstData > right.firstData)
		{
			return true;
		}
		else if (firstData < right.firstData)
		{
			return false;
		}
		if (secondData > right.secondData)
		{
			return true;
		}
		else if (secondData < right.secondData)
		{
			return false;
		}
		return false;
	}

	//表示哈希计算
	struct Hash
	{
		HX_CONSTEXPR17_FUNC size_t operator()(const HxUuid& uuid) const HX_NOEXCEPT
		{
			return HxHash_fnv1a((const unsigned char*)&uuid, sizeof(HxUuid));
		}
	};
#endif // __cplusplus
};

#ifdef __cplusplus
namespace HxLibs
{
	//定义全局标识。
	using Uuid = HxUuid;
}
namespace std
{
	//定义标准库哈希
	template <>
	struct hash<HxUuid> : public HxUuid::Hash
	{
		using result_type = size_t;
		using argument_type = HxUuid;
	};
}
#endif // __cplusplus

HX_INLINE void HxUuid_zero(HxUuid* v) HX_NOEXCEPT
{
	v->firstData = 0LL;
	v->secondData = 0LL;
}

#endif //!HX_UUID

#include "BufView.h"

#ifndef HX_UUIDS
#define HX_UUIDS

HX_BUFVIEW_DEFINE_ROOT_NS(Uuid)

#endif //HX_UUIDS

#endif //!HX_CORE_UUID_H
