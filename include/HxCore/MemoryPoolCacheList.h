/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_POOL_CACHE_LIST_H
#define HX_CORE_MEMORY_POOL_CACHE_LIST_H

#ifdef __cplusplus
#ifndef HX_MEMORY_POOL_CACHE_LIST
#define HX_MEMORY_POOL_CACHE_LIST

#include <type_traits>
#include "Defines.h"

namespace HxLibs
{
	//表示内存池缓存链表对象，其中内存缓存大小必须≥sizeof(void*)。
	class MemoryPoolCacheList : Uncopyable
	{
	private:
		//缓存数量
		size_t m_count{ 0 };
		//表示缓存条目
		struct Entry
		{
			Entry* prev;
		};
		//记录最末缓存
		Entry* m_last{ nullptr };

	public:
		//弹出一个缓存。
		HX_INLINE void* pop() HX_NOEXCEPT
		{
			auto ret = m_last;
			if (m_last != nullptr)
			{
				--m_count;
				m_last = m_last->prev;
			}
			return ret;
		}
		//保存一个缓存缓存。
		HX_INLINE void push(void* p) HX_NOEXCEPT
		{
			auto val = (Entry*)p;
			val->prev = m_last;
			m_last = val;
			++m_count;
		}

		//获取是否为空。
		HX_INLINE bool empty() const HX_NOEXCEPT
		{
			return m_count == 0;
		}
		//获取缓存上限。
		HX_INLINE size_t count() const HX_NOEXCEPT
		{
			return m_count;
		}

		//交换两上对象
		HX_INLINE void swap(MemoryPoolCacheList& other) HX_NOEXCEPT
		{
			::std::swap(m_last, other.m_last);
			::std::swap(m_count, other.m_count);
		}

		//默认构造函数。
		HX_INLINE MemoryPoolCacheList() HX_NOEXCEPT
		{
			//NOTHING
		}
	};
}
using HxMemoryPoolCacheList = ::HxLibs::MemoryPoolCacheList;

#endif //!HX_MEMORY_POOL_CACHE_LIST
#endif //__cplusplus

#endif // !HX_CORE_MEMORY_POOL_CACHE_LIST_H
