/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_PACKED_ALLOCATION_H
#define HX_CORE_MEMORY_PACKED_ALLOCATION_H

#ifdef __cplusplus
#ifndef HX_MEMORY_PACKED_ALLOCATION
#define HX_MEMORY_PACKED_ALLOCATION

#include <stdint.h>
#include <type_traits>

#include "BufView.h"
#include "Uncopyable.h"
#include "ByteLength.h"
#include "MemoryAlignment.h"
#include "MemoryReplicator.h"
#include "MemoryGlobalDefault.h"

namespace HxLibs
{
	/*
	* 调用示例代码
	*	//待分配缓存
	*	BufView<int> bufView1(nullptr, 10);
	*	BufView<float> bufView2(nullptr, 10);
	*	BufView<int*> bufView3(nullptr, 10);
	*	BufView<void> bufView4(nullptr, 10);
	*	//初始化空缓存
	*	SimpleBuffer<void> buffer;
	*	//初始化分配对象
	*	auto allocation = buffer.packing();
	*	//执行分配处理，第一次计算缓存大小并分配空间，第二次进行分配
	*	while (!allocation.assigned())
	*	{
	*		allocation.append(bufView1);
	*		allocation.append(bufView2);
	*		allocation.append(bufView3);
	*		allocation.append(bufView4);
	*		allocation.append(bufView5);
	*		//第一次真实分配空间，第二次仅作标记
	*		allocation.assign();
	*	}
	*/

	//表示方便的打包内存计算和分配功能。
	template <typename Global = MemoryGlobalDefault, size_t MaxAlignment = MemoryAlignment<void>::value>
	class MemoryPackedAllocation : Uncopyable
	{
		HX_STATIC_ASSERT((MaxAlignment >= MemoryAlignment<void>::value), "MemoryPackedAllocation<> Alignment parameter must >= MemoryAlignment<void>::value.");
		HX_STATIC_ASSERT((checkPowerOf2(MaxAlignment)), "MemoryPackedAllocation<> Alignment parameter must be a power of 2.");

	public:
		//枚举状态。
		enum class State
		{
			//计算缓存大小
			PREPARING = 0,
			//缓存申请待分配
			PREPARED = 1,
			//缓存已经分配
			ASSIGNED = 2,
		};

	private:
		uintptr_t& m_ptr;//起始地址
		size_t& m_size;//分配大小
		uintptr_t m_end;//结束位置，对于计算过程以0为起始，对于分配过程以起始地址为起点
		State m_state;//当前状态

	public:
		//获取占用大小。
		HX_INLINE size_t size() const HX_NOEXCEPT
		{
			return m_size;
		}
		//获取当前状态。
		HX_INLINE State state() const HX_NOEXCEPT
		{
			return m_state;
		}

		//构造函数。
		HX_INLINE MemoryPackedAllocation(uintptr_t& ptr, size_t& size)
			: m_ptr(ptr)
			, m_size(size)
			, m_end(MaxAlignment)
			, m_state(State::PREPARING)
		{
			//NOTHING
		}
		//移动构造函数。
		HX_INLINE MemoryPackedAllocation(MemoryPackedAllocation&& other)
			: m_ptr(other.m_ptr)
			, m_size(other.m_size)
			, m_end(other.m_end)
			, m_state(other.m_state)
		{
			//NOTHING
		}

		//添加需要分配的缓存（第一次调用计算大小，第二次调用分配缓存）
		template <typename T, size_t Alignment = MaxAlignment>
		HX_INLINE void append(T*& ptr, size_t size) HX_NOEXCEPT
		{
			HX_STATIC_ASSERT((checkPowerOf2(Alignment)), "MemoryPackedAllocation<>::append template argument Alignment must be power of 2.");
			HX_STATIC_ASSERT((Alignment <= MaxAlignment), "MemoryPackedAllocation<>::append template argument Alignment must be less than MaxAlignment.");

			size_t bytes = ByteLength<T>::count(size);
			if (m_state == State::PREPARING)
			{
				auto p = (m_end + (Alignment - 1U)) / Alignment * Alignment;
				m_end = p + bytes;
			}
			else if (m_state == State::PREPARED)
			{
				auto p = (m_end + (Alignment - 1U)) / Alignment * Alignment;
				m_end = p + bytes;
				ptr = (T*)p;
			}
		}
		//添加需要分配的缓存（第一次调用计算大小，第二次调用分配缓存）
		template <typename T, size_t Alignment = MaxAlignment>
		HX_INLINE void append(BufView<T>& bufView) HX_NOEXCEPT
		{
			append<T, Alignment>((T*&)bufView, bufView.size());
		}

		//执行内存分配处理(第一次调用申请缓存，第二次调用标记为已分配)。
		HX_INLINE void assign() HX_NOEXCEPT
		{
			if (m_state == State::PREPARING)
			{
				auto size = (size_t)m_end;
				if (size > m_size || size < m_size / 2)
				{//只有在原始容量不够或原始分配太大的情况下，才重新进行分配
					Global::deallocate((void*)m_ptr);
					m_ptr = (uintptr_t)(Global::allocate(size, MaxAlignment));
					m_size = m_ptr != 0 ? size : 0;
				}
				m_end = m_ptr;
				m_state = State::PREPARED;
			}
			else if (m_state == State::PREPARED)
			{
				m_state = State::ASSIGNED;
			}
		}
		//是否进行了内存分配。
		HX_INLINE bool assigned() const HX_NOEXCEPT
		{
			return m_state == State::ASSIGNED;
		}
	};
}

template <typename Global = HxMemoryGlobalDefault, size_t MaxAlignment = sizeof(void*)>
using HxMemoryPackedAllocation = ::HxLibs::MemoryPackedAllocation<Global, MaxAlignment>;

#endif //!HX_MEMORY_PACKED_ALLOCATION
#endif //__cplusplus

#endif //!HX_CORE_MEMORY_PACKED_ALLOCATION_H
