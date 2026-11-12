/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "MemoryPackedAllocation.h"
#include <HxCore/SimpleBuffer.h>
#include <cstdio>

namespace HxLibs
{
	void testMemoryPackedAllocation()
	{
		printf("testMemoryPackedAllocation begin!!\r\n");

		//待分配缓存
		BufView<int> bufView1(nullptr, 10);
		BufView<float> bufView2(nullptr, 10);
		BufView<int*> bufView3(nullptr, 10);
		BufView<void> bufView4(nullptr, 10);
		//初始化空缓存
		SimpleBuffer<void> buffer;
		//初始化分配对象
		auto allocation = buffer.packing();
		//执行分配处理，第一次计算缓存大小并分配空间，第二次进行分配
		while (!allocation.assigned())
		{
			allocation.append(bufView1);
			allocation.append(bufView2);
			allocation.append(bufView3);
			allocation.append(bufView4);
			//第一次真实分配空间，第二次仅作标记
			allocation.assign();
		}

		printf("testMemoryPackedAllocation ok!!\r\n");
	}
}
