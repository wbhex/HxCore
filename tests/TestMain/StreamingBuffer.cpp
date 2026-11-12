/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "StreamingBuffer.h"
#include <HxCore/WorkLoop.h>
#include <HxCore/Containers/Vector.h>
#include <HxCore/ByteStreamingBufferSegment.h>
#include <HxCore/CharStreamingBufferSegment.h>
#include <HxCore/NumberStreamingBufferSegment.h>
#include <HxCore/ComplexStreamingBufferSegment.h>

namespace HxLibs
{
	void testStreamingBuffer2()
	{
		WorkLoop workLoop;
		size_t counter = 0;
		StreamingBuffer<size_t> bufPool(4096 * 500, 1000);
		unsigned long long bufCount = 0;
		do
		{
			BufView<size_t> bufView;
			if (counter % 2 == 0)
			{
				bufView = bufPool.allocBlocks(1);
			}
			else
			{
				bufView = bufPool.allocate(bufPool.blockSize() + 100);
			}
			if (bufView.size() > 0)
			{
				for (size_t i = 0; i < bufView.size(); ++i)
				{
					bufView[i] = i;
				}
			}
			else
			{
				printf("StreamingBuffer2 alloc failed!\r\n");
				continue;
			}
			bufCount += bufView.size();
			if (counter % 6 == 0)
			{
				auto outView = bufPool.commitLockView(bufView);
				workLoop.push([outView, &bufPool]()
					{
						bufPool.unlockView(outView);
					});
			}
			else if (counter % 6 == 1)
			{
				auto segment = bufPool.commitLockSegment(bufView);
				workLoop.push([segment]()
					{
						unsigned long long from;
						auto bufView = segment->retrieve(from);
						auto outView = segment->lockExtraView(from, bufView.size());
						segment->unlockExtraView(outView);
					});
			}
			else
			{
				unsigned long long to = bufPool.commit(bufView);
				auto from = to - bufView.size();
				if (counter % 6 == 2)
				{
					workLoop.push([from, to, &bufPool]()
						{
							auto outView = bufPool.tryLockView(from, to);
							bufPool.unlockView(outView);
						});
				}
				else if (counter % 6 == 3)
				{
					workLoop.push([from, to, &bufPool]()
						{
							auto segment = bufPool.tryLockSegment(from, to);
							if (segment != nullptr)
							{
								segment->release();
							}
						});
				}
				else if (counter % 6 == 4)
				{
					workLoop.push([from, to, &bufPool]()
						{
							auto outView = bufPool.lockView(from, to);
							bufPool.unlockView(outView);
						});
				}
				else
				{
					workLoop.push([from, to, &bufPool]()
						{
							auto segment = bufPool.lockSegment(from, to);
							if (segment != nullptr)
							{
								segment->release();
							}
						});
				}
			}
			++counter;
		} while (bufCount >= bufPool.capacity() * 10);
		workLoop.exit();
		workLoop.join();
	}

	void testStreamingBuffer0()
	{
		size_t value = 0;
		StreamingBuffer<size_t> bufPool(4000, 500);
		while (value < bufPool.capacity() * 3)
		{
			auto bufView = bufPool.allocate(5000);
			for (size_t i = 0; i < bufView.size(); ++i, ++value)
			{
				bufView[i] = value;
			}
			bufPool.commit(bufView);
		}
		auto bufCView1 = bufPool.lockView(value - bufPool.capacity() + 2000, 1000);
		auto bufCView2 = bufPool.tryLockView(value - bufPool.capacity() + 3000, 1000);
		auto segment1 = bufPool.lockSegment(value - bufPool.capacity() + 4000, 8000);
		auto segment2 = bufPool.tryLockSegment(value - bufPool.capacity() + 12000, 8000);

		segment1->release();
		segment2->release();

		bufPool.unlockView(bufCView1);
		bufPool.unlockView(bufCView2);
	}


	void testStreamingBuffer()
	{
		testStreamingBuffer0();
		testStreamingBuffer2();
	}
}
