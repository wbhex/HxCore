/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "String.h"

namespace HxLibs
{
	void testString()
	{
		const char* nulltext = nullptr;
		const char* emptytext = "";
		const char* text = "012345678901234567890123456789";
		CString cstr(text);
		StringView sview(text);
		if (nulltext >= cstr)
		{
			throw ::std::exception();
		}
		if (emptytext >= cstr)
		{
			throw ::std::exception();
		}
		if (cstr != sview)
		{
			throw ::std::exception();
		}
		if (cstr != text)
		{
			throw ::std::exception();
		}
		if (sview != text)
		{
			throw ::std::exception();
		}
	}
}
