/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STREAMING_BUFFER_SLIM_H
#define HX_CORE_STREAMING_BUFFER_SLIM_H

#ifdef __cplusplus
#ifndef HX_STREAMING_BUFFER_SLIM
#define HX_STREAMING_BUFFER_SLIM

#include <assert.h>
#include <stdexcept>
#include "Range.h"
#include "BufView.h"
#include "StreamingBufferState.h"

namespace HxLibs
{
	//表示流式缓存精简对象（不持有缓存）。
	template <typename T, typename LockFlag = short>
	struct StreamingBufferSlim
	{
	private:
		BufView<T> m_bufView;//缓存视图
		BufView<LockFlag> m_lockView;//锁定视图：-1表示写锁定，0表示有数据但未锁定，＞0表示读取锁定
		const size_t m_blockSize;//分块大小
		size_t m_blockIndex{ 0 };//当前块位置，也即下一次写入数据的块位置
		size_t m_blockOffset{ 0 };//当前块偏移，也即下一次写入数据的偏移位置
		size_t m_globalIndex{ 0 };//当前全局位置，也即下一次写入数据的位置
		Hx64u m_allocFrom{ 0 }; //分配范围起始
		size_t m_allocBlockIndex{ 0 };//分配块位置，也即下一次写入数据的块位置
		size_t m_allocBlockOffset{ 0 };//分配块偏移，也即下一次写入数据的偏移位置
		size_t m_allocGlobalIndex{ 0 };//分配全局位置，也即下一次写入数据的位置
		Range<Hx64u> m_validRange{ 0, 0 };//当前有效范围，以全局计数表示

		//锁定从0到结束位置的区间。
		HX_INLINE void lockEnd(size_t end) HX_NOEXCEPT
		{
			for (size_t i = 0, l = (end - 1) / m_blockSize; i <= l; ++i)
			{
				++m_lockView[i];
			}
		}
		//锁定从起始位置开始的区间。
		HX_INLINE void lockStart(size_t start) HX_NOEXCEPT
		{
			for (size_t i = start / m_blockSize; i < m_lockView.size(); ++i)
			{
				++m_lockView[i];
			}
		}
		//锁定从起始位置到结束位置的区间。
		HX_INLINE void lockRange(size_t start, size_t end) HX_NOEXCEPT
		{
			for (size_t i = start / m_blockSize, l = (end - 1) / m_blockSize; i <= l; ++i)
			{
				++m_lockView[i];
			}
		}

		//尝试分配指定的块，如果分配成功则返回TRUE。
		HX_INLINE bool tryAllocBlock(size_t index, size_t& allocSize) HX_NOEXCEPT
		{
			auto& lockState = m_lockView[index];
			if (lockState > 0)
			{//被锁定
				return false;
			}
			else if (lockState == 0)
			{//存在数据但可写入
				allocSize += m_blockSize;
				m_validRange.shrink(m_blockSize);
				lockState = -1;
			}
			else
			{//不存在数据
				allocSize += m_blockSize;
			}
			return true;
		}

	public:
		//初始化当前对象。
		HX_INLINE StreamingBufferSlim() HX_NOEXCEPT
			: m_blockSize(0)
		{
			//NOTHING
		}
		//初始化当前对象。
		HX_INLINE StreamingBufferSlim(const BufView<T>& bufView, const BufView<LockFlag>& lockView, size_t blockSize) HX_NOEXCEPT
			: m_bufView(bufView)
			, m_lockView(lockView)
			, m_blockSize(blockSize)
		{
			assert(lockView.size() * blockSize == bufView.size());
			for (size_t i = 0; i < m_lockView.size(); ++i)
			{
				m_lockView[i] = -1;
			}
		}

		//获取当前范围。
		HX_INLINE const Range<Hx64u>& range() const HX_NOEXCEPT
		{
			return m_validRange;
		}

		//获取缓存容量。
		HX_INLINE size_t capacity() const HX_NOEXCEPT
		{
			return m_bufView.size();
		}
		//获取分块大小。
		HX_INLINE size_t blockSize() const HX_NOEXCEPT
		{
			return m_blockSize;
		}
		//获取分块数量。
		HX_INLINE size_t blockCount() const HX_NOEXCEPT
		{
			return m_lockView.size();
		}

		//获取可用大小。
		HX_INLINE size_t availableSize() const HX_NOEXCEPT
		{
			size_t value = 0;
			auto blockStartIndex = m_blockIndex;
			if (m_blockOffset != 0)
			{
				value = m_blockSize - m_blockOffset;
				++blockStartIndex;
			}
			for (auto i = blockStartIndex; i < m_lockView.size(); ++i)
			{
				if (m_lockView[i] <= 0)
				{
					value += m_blockSize;
				}
				else
				{
					return value;
				}
			}
			for (size_t i = 0; i < m_blockIndex; ++i)
			{
				if (m_lockView[i] <= 0)
				{
					value += m_blockSize;
				}
				else
				{
					return value;
				}
			}
			return value;
		}

		//申请指定大小的缓存片段以进行写入，如果可写空间在尾部且跨越头部，则将仅返回尾部区间视图，如果返回长度为0表示当前无可写空间。
		HX_INLINE BufView<T> allocate(size_t size, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			//起始赋值
			if (from != nullptr)
			{
				*from = m_allocFrom;
			}
			//长度为0则直接返回
			if (size == 0)
			{
				return BufView<T>{};
			}
			//起始非整数块分配
			size_t allocSize = 0;
			if (m_allocBlockOffset != 0)
			{
				allocSize = m_blockSize - m_allocBlockOffset;
				if (allocSize > size)
				{//可以满足需求则直接返回
					auto globalIndex = m_allocGlobalIndex;
					{
						m_allocFrom += size;
						m_allocBlockOffset += size;
						m_allocGlobalIndex += size;
					}
					return BufView<T>(m_bufView + globalIndex, size);
				}
				else if (allocSize == size)
				{//刚好满足需求则直接返回
					auto globalIndex = m_allocGlobalIndex;
					{
						m_allocFrom += size;
						m_allocBlockOffset = 0;
						++m_allocBlockIndex;
						if (m_allocBlockIndex == m_lockView.size())
						{//为末块
							m_allocBlockIndex = 0;
							m_allocGlobalIndex = 0;
						}
						else
						{//不为末块
							m_allocGlobalIndex += size;
						}
					}
					return BufView<T>(m_bufView + globalIndex, size);
				}
				else
				{//不满足大小，则后续再分配整数块
					++m_allocBlockIndex;
					m_allocBlockOffset = 0;
				}
			}
			//整数块分配
			while (allocSize < size && m_allocBlockIndex < m_lockView.size())
			{
				if (!tryAllocBlock(m_allocBlockIndex, allocSize))
				{
					break;
				}
				++m_allocBlockIndex;
			}
			//根据分配情况进行处理
			if (allocSize > size)
			{//分配成功，但大于需求大小
				auto globalIndex = m_allocGlobalIndex;
				{
					m_allocFrom += size;
					--m_allocBlockIndex;
					m_allocBlockOffset = m_blockSize - (allocSize - size);
					m_allocGlobalIndex += size;
				}
				return BufView<T>(m_bufView + globalIndex, size);
			}
			else if (allocSize == size)
			{//分配成功，刚好等于需求大小
				auto globalIndex = m_allocGlobalIndex;
				{
					m_allocFrom += size;
					if (m_allocBlockIndex == m_lockView.size())
					{//为末块
						m_allocBlockIndex = 0;
						m_allocGlobalIndex = 0;
					}
					else
					{//不为末块
						m_allocGlobalIndex += size;
					}
				}
				return BufView<T>(m_bufView + globalIndex, size);
			}
			else if (allocSize > 0)
			{//分配成功，但小于需求大小
				auto globalIndex = m_allocGlobalIndex;
				{
					m_allocFrom += allocSize;
					if (m_allocBlockIndex == m_lockView.size())
					{//为末块
						m_allocBlockIndex = 0;
						m_allocGlobalIndex = 0;
					}
					else
					{//不为末块
						m_allocGlobalIndex += allocSize;
					}
				}
				return BufView<T>(m_bufView + globalIndex, allocSize);
			}
			else
			{//分配失败
				return BufView<T>{};
			}
		}
		HX_INLINE BufView<T> allocate(size_t size, Hx64u& from) HX_NOEXCEPT
		{
			return allocate(size, &from);
		}

		//申请指定数量的缓存块以进行写入，如果可写空间在尾部且跨越头部，则将仅返回尾部区间视图，如果返回长度为0表示当前无可写空间。
		HX_INLINE BufView<T> allocBlocks(size_t count = 1, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			//起始赋值
			if (from != nullptr)
			{
				*from = m_allocFrom;
			}
			//块数量为0或者不能按整数块进行分配则直接返回
			if (count == 0 || m_allocBlockOffset != 0)
			{
				return BufView<T>{};
			}
			//执行整数块分配
			size_t allocSize = 0;
			for (size_t i = 0; i < count && m_allocBlockIndex < m_lockView.size(); ++i, ++m_allocBlockIndex)
			{
				if (!tryAllocBlock(m_allocBlockIndex, allocSize))
				{
					break;
				}
			}
			if (allocSize > 0)
			{//分配成功
				auto globalIndex = m_allocGlobalIndex;
				{
					m_allocFrom += allocSize;
					if (m_allocBlockIndex == m_lockView.size())
					{//为末块
						m_allocBlockIndex = 0;
						m_allocGlobalIndex = 0;
					}
					else
					{//不为末块
						m_allocGlobalIndex += allocSize;
					}
				}
				return BufView<T>(m_bufView + globalIndex, allocSize);
			}
			else
			{//分配失败
				return BufView<T>{};
			}
		}
		HX_INLINE BufView<T> allocBlocks(size_t count, Hx64u& from) HX_NOEXCEPT
		{
			return allocBlocks(count, &from);
		}

		//清除之前的申请记录，后续从写入头开始重新申请缓存。
		HX_INLINE void reallocate() HX_NOEXCEPT
		{
			m_allocFrom = m_validRange.to();
			m_allocBlockIndex = m_blockIndex;
			m_allocBlockOffset = m_blockOffset;
			m_allocGlobalIndex = m_globalIndex;
		}

		//将指定的写入数据应用到缓存池中并返回总共数据量（应用的缓存必须是之前申请的缓存，大小可以小于申请缓存大小）。
		HX_INLINE Hx64u commit(const BufView<T>& view) HX_NOEXCEPT
		{
			assert(view.ptr() == m_bufView + m_globalIndex);//不为之前申请的缓存片段断言
			assert(view.size() <= m_bufView.size() - m_globalIndex);//长度异常断言
			assert(m_validRange.to() + view.size() <= m_allocFrom);//长度异常断言;

			//输入有效性检查
			auto size = view.size();
			if (size == 0)
			{//长度异常则直接返回
				return m_validRange.to();
			}
			//起始非整块应用
			if (m_blockOffset != 0)
			{
				auto needSize = m_blockSize - m_blockOffset;
				if (size >= needSize)
				{//可填充满
					++m_blockIndex;
					m_blockOffset = 0;
					size -= needSize;
				}
				else
				{//未填充满，改写状态并返回
					m_blockOffset += size;
					m_validRange.extend(size);
					m_globalIndex += size;
					return m_validRange.to();
				}
			}
			//中间整块应用
			auto blockCount = size / m_blockSize;
			while (blockCount > 0)
			{
				auto& lockState = m_lockView[m_blockIndex];
				assert(lockState == -1);//非写块断言
				lockState = 0;
				--blockCount;
				++m_blockIndex;
				size -= m_blockSize;
			}
			//末尾非整块应用
			if (size > 0)
			{
				auto& lockState = m_lockView[m_blockIndex]; 
				assert(lockState == -1); //非写块断言
				lockState = 0;
				m_blockOffset = size;
			}
			//改写状态并返回
			if (m_blockIndex == m_lockView.size())
			{//尾部已经写完
				m_blockIndex = 0;
				m_globalIndex = 0;
			}
			else
			{//尾部未写完
				m_globalIndex += view.size();
			}
			m_validRange.extend(view.size());
			return m_validRange.to();
		}

		//将指定的写入数据应用到缓存池中并锁定该缓存区域视图（应用的缓存必须是之前申请的缓存，大小可以小于申请缓存大小）。
		HX_INLINE BufConstView<T> commitLockView(const BufView<T>& view, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			assert(view.size() > 0);//长度为0断言
			assert(view.ptr() == m_bufView + m_globalIndex);//不为之前申请的缓存片段断言
			assert(view.size() <= m_bufView.size() - m_globalIndex);//长度异常断言

			//起始赋值
			if (from != nullptr)
			{
				*from = m_validRange.to();
			}
			auto size = view.size();
			//起始非整块应用
			if (m_blockOffset != 0)
			{
				++m_lockView[m_blockIndex];
				auto needSize = m_blockSize - m_blockOffset;
				if (size >= needSize)
				{//可填充满
					++m_blockIndex;
					m_blockOffset = 0;
					size -= needSize;
				}
				else
				{//未填充满，改写状态并返回
					m_blockOffset += size;
					m_validRange.extend(size);
					m_globalIndex += size;
					return view;
				}
			}
			//中间整块应用
			auto blockCount = size / m_blockSize;
			while (blockCount > 0)
			{
				auto& lockState = m_lockView[m_blockIndex];
				assert(lockState == -1); //非写块断言
				lockState = 1;
				--blockCount;
				++m_blockIndex;
				size -= m_blockSize;
			}
			//末尾非整块应用
			if (size > 0)
			{
				auto& lockState = m_lockView[m_blockIndex];
				assert(lockState == -1); //非写块断言
				lockState = 1;
				m_blockOffset = size;
			}
			//改写状态并返回
			if (m_blockIndex == m_lockView.size())
			{//尾部已经写完
				m_blockIndex = 0;
				m_globalIndex = 0;
			}
			else
			{//尾部未写完
				m_globalIndex += view.size();
			}
			m_validRange.extend(view.size());
			return view;
		}
		HX_INLINE BufConstView<T> commitLockView(const BufView<T>& view, Hx64u& from) HX_NOEXCEPT
		{
			return commitLockView(view, &from);
		}

		//锁定指定区间的缓存视图以进行读取并返回实际的区间起始，如果指定区间不能锁定将会缩小锁定的区间范围：
		//	如果缓存区间在缓存池尾部和首部，view2将为首部区间视图，否则为空;
		//	如果view2参数为空，则锁定的大小不会包含首部区间视图。
		HX_INLINE Hx64u lockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			auto to = from + size;
			if (from < m_validRange.from())
			{//小于起始，则取起始
				from = m_validRange.from();
			}
			if (to > m_validRange.to())
			{//大于结束，则取结束
				to = m_validRange.to();
			}
			if (from >= to)
			{//超出范围
				view1.reset(nullptr, 0);
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
			}
			else
			{
				size = to - from;
				auto start = from % m_bufView.size();
				auto end = start + size;
				if (end > m_bufView.size())
				{//需要取尾部+首部
					lockStart(start);
					view1.reset(m_bufView + start, m_bufView.size() - start);
					if (view2 != nullptr)
					{
						view2->reset(m_bufView.ptr(), size - view1.size());
						lockEnd(view2->size());
					}
				}
				else
				{//仅需要尾部
					lockRange(start, end);
					view1.reset(m_bufView + start, size);
					if (view2 != nullptr)
					{
						view2->reset(nullptr, 0);
					}
				}
			}
			return from;
		}
		HX_INLINE Hx64u lockView(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			return lockView(from, size, *view1, view2);
		}
		HX_INLINE Hx64u lockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>& view2) HX_NOEXCEPT
		{
			return lockView(from, size, view1, &view2);
		}
		HX_INLINE BufConstView<T> lockView(Hx64u from, size_t size) HX_NOEXCEPT
		{
			BufConstView<T> view;
			lockView(from, size, view);
			return view;
		}
		HX_INLINE BufConstView<T> lockView(Hx64u from, size_t size, Hx64u& fromActual) HX_NOEXCEPT
		{
			BufConstView<T> view;
			fromActual = lockView(from, size, view);
			return view;
		}
		//尝试锁定指定区间的缓存视图以进行读取，如果指定的区间不满足读取条件将返回0，否则返回区间大小。
		//	如果缓存区间在缓存池尾部和首部，view2将为首部区间视图，否则为空。
		//	如果view2参数为空且锁定的区间包含首部区间时将不会锁定该首部区间，返回大小只包含缓存池尾部区间。
		HX_INLINE size_t tryLockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			auto to = from + size;
			if (from < m_validRange.from() || to > m_validRange.to())
			{//要求的区间小于起始或大于结束
				view1.reset(nullptr, 0);
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				return 0;
			}
			auto start = from % m_bufView.size();
			auto end = start + size;
			if (end > m_bufView.size())
			{//需要取尾部+首部
				lockStart(start);
				view1.reset(m_bufView + start, m_bufView.size() - start);
				if (view2 != nullptr)
				{
					view2->reset(m_bufView.ptr(), size - view1.size());
					lockEnd(view2->size());
					return size;
				}
				return view1.size();
			}
			else
			{//仅需要尾部
				lockRange(start, end);
				view1.reset(m_bufView + start, size);
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				return size;
			}
		}
		HX_INLINE size_t tryLockView(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			return tryLockView(from, size, *view1, view2);
		}
		HX_INLINE size_t tryLockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>& view2) HX_NOEXCEPT
		{
			return tryLockView(from, size, view1, &view2);
		}
		HX_INLINE BufConstView<T> tryLockView(Hx64u from, size_t size) HX_NOEXCEPT
		{
			BufConstView<T> view;
			tryLockView(from, size, view, nullptr);
			return view;
		}
		//释放之前被锁定的缓存视图。
		HX_INLINE void unlockView(const BufConstView<T>& view) HX_NOEXCEPT
		{//TODO 需要增加输入检查
			auto start = view - m_bufView.ptr();
			auto end = start + view.size();
			for (size_t i = start / m_blockSize, l = (end - 1) / m_blockSize; i <= l; ++i)
			{
				auto& lockState = m_lockView[i];
				assert(lockState > 0);//未锁定断言
				--lockState;
			}
		}

		//查询缓存池当前状态。
		HX_INLINE void queryState(StreamingBufferState& value) const HX_NOEXCEPT
		{
			value.capacity = capacity();
			value.blockSize = blockSize();
			value.validFrom = m_validRange.from();
			value.validSize = m_validRange.size();
			value.availableSize = availableSize();
		}
		HX_INLINE void queryState(StreamingBufferState* value) const HX_NOEXCEPT
		{
			return queryState(*value);
		}
	};
}
template <typename T, typename LockFlag = short>
using HxStreamingBufferSlim = ::HxLibs::StreamingBufferSlim<T, LockFlag>;

#endif //!HX_STREAMING_BUFFER_SLIM
#endif //__cplusplus

#endif //!HX_CORE_STREAMING_BUFFER_SLIM_H
