/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "SharedObjectPool.h"

#include <HxCore/Uuid.h>
#include <HxCore/Time.h>
#include <HxCore/Complex.h>
#include <HxCore/Fraction.h>
#include <HxCore/SimpleBuffer.h>
#include <HxCore/Containers/Deque.h>
#include <HxCore/Containers/Queue.h>
#include <HxCore/Containers/Stack.h>
#include <HxCore/Containers/Vector.h>
#include <HxCore/Containers/HashMap.h>
#include <HxCore/Containers/HashSet.h>
#include <HxCore/Containers/RBTreeMap.h>
#include <HxCore/Containers/RBTreeSet.h>
#include <HxCore/Containers/LinkedList.h>

namespace HxLibs
{
	void testSharedObjectPool()
	{
		struct Obj1
		{
			size_t size;
			char data[1020];
			Vector<Time> times;
			LinkedList<Uuid> uuids;
		};
		struct Obj2
		{
			Time time;
			Uuid uuid;
			HashSet<Uuid> hashSet;
		};
		struct Obj3
		{
			Time time;
			Uuid uuid;
			Stack<Hx32fc> stack;
			Queue<Fraction<long>> queue;
			RBTreeSet<Uuid> treeSet;
			RBTreeMap<Uuid, Time> treeMap;
		};

		Vector<::std::shared_ptr<void>> objItems;

		SharedObjectPool0<Obj1> objPool1;
		objItems.resize(20);
		for (size_t i = 0; i < objItems.size(); ++i)
		{
			objItems[i] = objPool1.share();
		}
		objItems.resize(10);
		objPool1.setCache(objItems.size() / 4);
		objItems.resize(0);
		objPool1.setCache(0);

		SharedObjectPool1<Obj2> objPool2;
		objItems.resize(20);
		for (size_t i = 0; i < objItems.size(); ++i)
		{
			objItems[i] = objPool2.share();
		}
		objItems.resize(10);
		objPool2.setCache(objItems.size() / 4);
		objItems.resize(0);
		objPool2.setCache(0);

		SharedObjectPool1<Obj3> objPool3;
		objItems.resize(30);
		for (size_t i = 0; i < objItems.size(); ++i)
		{
			objItems[i] = objPool3.share();
		}
		objItems.resize(10);
		objPool3.setCache(objItems.size() / 4);
		objItems.resize(0);
		objPool3.setCache(0);
	}
}
