/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_CONTAINERS_STACK_H
#define HX_CORE_CONTAINERS_STACK_H

#ifdef __cplusplus
#ifndef HX_CONTAINERS_STACK
#define HX_CONTAINERS_STACK

#include <stack>
#include "Deque.h"

namespace HxLibs
{
	namespace Containers
	{
		template <typename T, typename Container = Deque<T>>
		using Stack = ::std::stack<T, Container>;
	}
	template <typename T, typename Container = Deque<T>>
	using Stack = ::std::stack<T, Container>;
}
template <typename T, typename Container = HxDeque<T>>
using HxStack = ::std::stack<T, Container>;

#endif //!HX_CONTAINERS_STACK
#endif //__cplusplus

#endif //!HX_CORE_CONTAINERS_STACK_H
