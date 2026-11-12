/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "Fraction.h"
#include <HxCore/Fraction.h>

namespace HxLibs
{
	void testFraction()
	{
		Fraction<int> fraction1(3, 2, true);
		Fraction<unsigned int> fraction2(3, 2, true);

		HxFrac64u frac1(222, 6);
		HxFrac64u frac2(333, 3);
		auto frecc = frac1 - frac2;
	}
}
