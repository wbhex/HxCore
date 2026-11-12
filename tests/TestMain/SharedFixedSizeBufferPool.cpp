/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "SharedFixedSizeBufferPool.h"
#include <HxCore/Containers/Vector.h>

namespace HxLibs
{
	void testSharedFixedSizeBufferPool()
	{
		Vector<::std::shared_ptr<void>> bufItems;

		SharedFixedSizeBufferPool0<long long> bufPool1(1000);
		bufItems.resize(20);
		for (size_t i = 0; i < bufItems.size(); ++i)
		{
			bufItems[i] = bufPool1.share();
		}
		bufItems.resize(10);
		bufPool1.setCache(bufItems.size() / 4);
		bufItems.resize(0);
		bufPool1.setCache(0);

		SharedFixedSizeBufferPool1<char> bufPool2(1024 * 1024 * 2);
		bufItems.resize(20);
		for (size_t i = 0; i < bufItems.size(); ++i)
		{
			bufItems[i] = bufPool2.share();
		}
		bufItems.resize(10);
		bufPool2.setCache(bufItems.size() / 4);
		bufItems.resize(0);
		bufPool2.setCache(0);
	}
}
