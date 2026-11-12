/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "Uuid.h"
#include <HxCore/UuidCreate.h>
#include <HxCore/UuidString.h>

namespace HxLibs
{
	void testUuid()
	{
		auto uuid = HxUuid_create();
		auto text = toString(uuid);
	}
}
