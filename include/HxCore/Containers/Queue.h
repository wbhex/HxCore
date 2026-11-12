/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_QUEUE_H
#define HX_CORE_CONTAINERS_QUEUE_H

#ifdef __cplusplus
#ifndef HX_CONTAINERS_QUEUE
#define HX_CONTAINERS_QUEUE

#include <queue>
#include "Deque.h"

namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Container = Deque<T>>
		using Queue = ::std::queue<T, Container>;
	}
	template <typename T, typename Container = Deque<T>>
	using Queue = ::std::queue<T, Container>;
}
template <typename T, typename Container = HxDeque<T>>
using HxQueue = ::std::queue<T, Container>;

#endif //!HX_CONTAINERS_QUEUE
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_QUEUE_H
