/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "SimpleBufferStreamingView.h"
#include <HxCore/SimpleBuffer.h>

namespace HxLibs
{
	void testSimpleBufferStreamingView()
	{
		const size_t CAPACITY = 10240;
		const size_t MAX_VALUE = CAPACITY * 10000;
		const size_t MAX_BLOCK_SIZE = 40;
		const size_t ALLOC_BLOCK_SIZE = 17;
		const size_t HxITE_BLOCK_SIZE = 19;
		const size_t READ_BLOCK_SIZE = ALLOC_BLOCK_SIZE + HxITE_BLOCK_SIZE;

		SimpleBuffer<Hx64u, MemoryGlobalDefault> simpleBuffer(CAPACITY + MAX_BLOCK_SIZE);
		SimpleBufferStreamingView<Hx64u, MemoryReplicator> streamingView(simpleBuffer.ptr(), CAPACITY, MAX_BLOCK_SIZE);

		Hx64u value = 0;
		SimpleBuffer<Hx64u, MemoryGlobalDefault> writeBuffer(CAPACITY);
		printf("testSimpleBufferStreamingView begin!!\r\n");
		while (value < MAX_VALUE)
		{
			auto bufView = streamingView.alloc(ALLOC_BLOCK_SIZE);
			for (size_t i = 0; i < ALLOC_BLOCK_SIZE; ++i, ++value)
			{
				bufView[i] = value;
			}
			streamingView.apply(bufView);

			for (size_t i = 0; i < HxITE_BLOCK_SIZE; ++i, ++value)
			{
				writeBuffer[i] = value;
			}
			streamingView.write(writeBuffer.ptr(), HxITE_BLOCK_SIZE);

			auto valBeg = value - READ_BLOCK_SIZE;
			auto valView = streamingView.fetch(valBeg, READ_BLOCK_SIZE);
			for (size_t i = 0; i < READ_BLOCK_SIZE; ++i, ++valBeg)
			{
				if (valView[i] != valBeg)
				{
					printf("testSimpleBufferStreamingView failed!!\r\n");
					break;
				}
			}
		}
		streamingView.reset((BufView<Hx64u>)simpleBuffer, CAPACITY, MAX_BLOCK_SIZE);
		for (size_t i = 0; i < CAPACITY; ++i)
		{
			writeBuffer[i] = i;
		}
		streamingView.write(writeBuffer);

		for (size_t i = 0; i < MAX_BLOCK_SIZE; ++i)
		{
			writeBuffer[i] = CAPACITY + i;
		}
		streamingView.write(writeBuffer.ptr(), MAX_BLOCK_SIZE);

		auto block = streamingView.getBlock(CAPACITY - 10, MAX_BLOCK_SIZE);

		for (size_t i = 0; i < MAX_BLOCK_SIZE - 10; ++i)
		{
			block[i] = 65536;
		}
		streamingView.confirm(BufView<Hx64u>(block.ptr(), MAX_BLOCK_SIZE - 10));

		printf("testSimpleBufferStreamingView ok!!\r\n");
	}
}
