/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "Time.h"

namespace HxLibs
{
	void testTime()
	{
		Time now = Time::now();
		auto val = now.value();
		Time time(1980, 0);
		bool xxx = now > time;
		auto span = now - time;
	}
}
