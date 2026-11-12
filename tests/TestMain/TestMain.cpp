/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include <HxCore/Object.h>

#include <HxCore/Span.h>
#include <HxCore/Range.h>
#include <HxCore/RefCnt.h>
#include <HxCore/ObjRef.h>
#include <HxCore/Number.h>
#include <HxCore/Complex.h>
#include <HxCore/Stopwatch.h>
#include <HxCore/WorkLoop.h>

#include <HxCore/DllModule.h>

#define HX_DLL_FILE_PATH_IMPL
#include <HxCore/DllFilePath.h>
#define HX_EXE_FILE_PATH_IMPL
#include <HxCore/ExeFilePath.h>

#include <HxCore/MemoryReplicator.h>
#include <HxCore/MemoryGlobalDefault.h>
#include <HxCore/MemoryGlobalAligned.h>
#include <HxCore/MemoryGlobalResource.h>
#include <HxCore/MemoryGlobalAllocator.h>
#include <HxCore/MemoryPackedAllocation.h>

#include <HxCore/ArrayView.h>

#include <HxCore/SimpleBuffer.h>

#include <HxCore/FileIO.h>
#include <HxCore/StdFileIO.h>
#if HX_WINDOWS
#include <HxCore/WinDirectFileIO.h>
#endif //!HX_WINDOWS

#include <HxCore/Containers/Deque.h>
#include <HxCore/Containers/Queue.h>
#include <HxCore/Containers/Stack.h>
#include <HxCore/Containers/Vector.h>
#include <HxCore/Containers/HashSet.h>
#include <HxCore/Containers/HashMap.h>
#include <HxCore/Containers/RBTreeSet.h>
#include <HxCore/Containers/RBTreeMap.h>
#include <HxCore/Containers/LinkedList.h>

#include "Uuid.h"
#include "Time.h"
#include "String.h"
#include "Fraction.h"
#include "FixedSizeBufferPool.h"
#include "StreamingBuffer.h"
#include "SharedObjectPool.h"
#include "SharedFixedSizeBufferPool.h"
#include "SimpleBufferStreamingView.h"
#include "MemoryPackedAllocation.h"

namespace HxLibs
{
	static MemoryGlobalResource<> g_memoryGlobalResource;

	void testSimpleBuffer()
	{
		SimpleBuffer<Hx32fc> buffer(256);
		auto ptr1 = buffer + 128;
		auto val1 = buffer[64];
	}

	void testFileIO()
	{
		StdFileIO stdFileIO;
#if HX_WINDOWS
		WinFileIO winFileIO;
		WinDirectFileIO winDirectFileIO;
#endif //HX_WINDOWS
	}

	void testMain()
	{
		testUuid();
		testTime();
		testString();
		testFraction();

		testSimpleBuffer();

		testFileIO();

		testMemoryPackedAllocation();
		testSimpleBufferStreamingView();
		testFixedSizeBufferPool();
		testStreamingBuffer();
		testSharedObjectPool();
		testSharedFixedSizeBufferPool();

		::std::mutex mutex1;
		LockGuard<::std::mutex> lockGuard1(mutex1);
		::std::mutex mutex2;
		SpinLockGuard<::std::mutex> lockGuard2(mutex2);

		SimpleAtomicLocker atomicLocker0; atomicLocker0.lock();
		SecuredAtomicLocker<true> atomicLocker1; atomicLocker1.lock();
		SecuredAtomicLocker<false> atomicLocker2; atomicLocker2.lock();

		Complex<float> complex;
		DllModule dynamicLinkLibrary;
		ObjRef<Referencable> objRef;
		ObjectPool<Uuid> objectPool;
		auto uuid2 = objectPool.share0();

		Range<int> range;
		MemoryGlobalAllocator<int> rawMemoryAllocator;
		MemoryReplicator rawMemoryReplicator;
		RefCnt refCnt;

		SimpleBuffer<int, MemoryGlobalDefault> simpleBuffer1;
		simpleBuffer1.realloc(1024);

		SimpleBuffer<int, MemoryGlobalAligned> simpleBuffer2;
		simpleBuffer2.realloc(4096 * 10);

		Span<int> span;
		Stopwatch stopwatch;
		String str1;
		CString str2; 
		Uuid uuid;
		WorkLoop workLoop("TEST_WORK_LOOP");

		HxThread_sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	HxArrayView<HxUuid> uuidArray;
	HxArrayConstView<HxUuid> uuidConstArray(uuidArray);

	HxSharedFixedSizeBufferPool0<Hx16sc> bufferPool0;
	bufferPool0.setSize(1024 * 1024);
	bufferPool0.setCache(1024);
	auto bufferItem0 = bufferPool0.share();

	HxSharedFixedSizeBufferPool1<Hx16sc> bufferPool1;
	bufferPool1.setSize(1024 * 1024);
	bufferPool1.setCache(1024);
	auto bufferItem1 = bufferPool1.share();

	::HxLibs::testMain();

	return 0;
}
