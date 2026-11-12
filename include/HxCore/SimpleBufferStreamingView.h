/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_SIMPLE_BUFFER_STREAMING_VIEW_H
#define HX_CORE_SIMPLE_BUFFER_STREAMING_VIEW_H

#ifdef __cplusplus
#ifndef HX_SIMPLE_BUFFER_STREAMING_VIEW
#define HX_SIMPLE_BUFFER_STREAMING_VIEW

#include <assert.h>
#include <stdexcept>
#include "Span.h"
#include "Range.h"
#include "BufView.h"
#include "MemoryReplicator.h"

namespace HxLibs
{
	//表示简单缓存流式视图（不持有，支持一定的回溯访问）。
	template <typename T, typename Replicator = MemoryReplicator>
	struct SimpleBufferStreamingView
	{
	private:
		BufView<T> m_bufView;
		size_t m_capacity;//缓存容量
		size_t m_maxBlockSize;//处理的最大块尺寸
		size_t m_currentIndex;//当前数据结束位置，也即下一次写入数据的位置
		Range<Hx64u> m_currentRange;//当前数据范围，以全局计数表示
		mutable Range<Hx64u> m_extendedRange;//扩展数据表示范围，以全局计数表示

		//映射指定区间的地址，其中的参数都已经检验，满足要求。
		HX_INLINE T* mapAddress(Hx64u from, Hx64u to, size_t size) const HX_NOEXCEPT
		{
			size_t beginIndex = (size_t)(from % m_capacity);//通过求余计算起始索引
			size_t endIndex = beginIndex + size;
			if (endIndex > m_capacity)
			{//要读取的数据超出容量范围，则需要将头部数据填充到扩展区以进行返回
				auto extSize = endIndex - m_capacity;//扩展空间大小
				auto extStart = from + (m_capacity - beginIndex);//扩展空间起始
				if (m_extendedRange.from() == extStart)
				{//扩展空间内容有效，只扩展之前未扩展的内容
					if (m_extendedRange.to() < to)
					{//扩展之前未扩展的内容
						size_t extExistSize = (size_t)m_extendedRange.size();
						size_t extNeedCopySize = (size_t)(to - m_extendedRange.to());
						Replicator::replicate(m_bufView.ptr(), extExistSize, m_bufView.ptr(), m_capacity + extExistSize, extNeedCopySize);
						m_extendedRange.reset(extStart, to);
					}
				}
				else
				{//扩展空间内容无效，直接扩展即可
					Replicator::replicate(m_bufView.ptr(), m_bufView.ptr(), m_capacity, extSize);
					m_extendedRange.reset(extStart, extStart + extSize);
				}
			}
			return m_bufView.ptr() + beginIndex;
		}

	public:
		//初始化当前对象。
		HX_INLINE SimpleBufferStreamingView() HX_NOEXCEPT
			: m_capacity(0)
			, m_maxBlockSize(0)
			, m_currentIndex(0)
			, m_currentRange(0, 0)
			, m_extendedRange(0, 0)
		{
			//NOTHING
		}
		//初始化当前对象。
		HX_INLINE SimpleBufferStreamingView(T* bufPtr, size_t capacity, size_t maxBlockSize) HX_NOEXCEPT
			: m_bufView(bufPtr, capacity + maxBlockSize)
			, m_capacity(capacity)
			, m_maxBlockSize(maxBlockSize)
			, m_currentIndex(0)
			, m_currentRange(0, 0)
			, m_extendedRange(0, 0)
		{
			assert(maxBlockSize <= capacity);
		}
		//初始化当前对象。
		HX_INLINE SimpleBufferStreamingView(const BufView<T>& bufView, size_t capacity, size_t maxBlockSize) HX_NOEXCEPT
			: m_bufView(bufView)
			, m_capacity(capacity)
			, m_maxBlockSize(maxBlockSize)
			, m_currentIndex(0)
			, m_currentRange(0, 0)
			, m_extendedRange(0, 0)
		{
			assert(maxBlockSize <= capacity);
			assert(bufView.size() >= capacity + maxBlockSize);
		}

		//获取当前范围。
		HX_INLINE const Range<Hx64u>& range() const HX_NOEXCEPT
		{
			return m_currentRange;
		}

		//获取缓存容量。
		HX_INLINE size_t capacity() const HX_NOEXCEPT
		{
			return m_capacity;
		}
		//获取最大块大小。
		HX_INLINE size_t maxBlockSize() const HX_NOEXCEPT
		{
			return m_maxBlockSize;
		}

		//分配一块缓存进行写操作，最大不超过最大块大小，写完成后必须调用apply函数。
		HX_INLINE BufView<T> alloc(size_t size) HX_NOEXCEPT
		{
			assert(size <= m_maxBlockSize);
			return BufView<T>(m_bufView.ptr(), m_currentIndex, size);
		}
		//应用指定的写缓存块。
		HX_INLINE void apply(const BufView<T>& bufView) HX_NOEXCEPT
		{
			assert(bufView.size() <= m_maxBlockSize);
			assert(bufView.ptr() == m_bufView + m_currentIndex);
			//更新当前数据范围
			if (m_currentRange.to() < m_capacity)
			{//小于容量，则直接延长
				m_currentRange += bufView.size();
				if (m_currentRange.to() > m_capacity)
				{//容量已经满，更新起始
					m_currentRange.reset(m_currentRange.to() - m_capacity, m_currentRange.to());
				}
			}
			else
			{//直接进行平移
				m_currentRange >>= bufView.size();
			}
			//更新当前数据信息
			m_currentIndex += bufView.size();
			if (m_currentIndex == m_capacity)
			{//写入到末尾，直接跳到头部
				m_currentIndex = 0;
			}
			else if (m_currentIndex > m_capacity)
			{//使用了扩展区域，则将扩展区域进行复制并同时标记扩展区域
				m_currentIndex = m_currentIndex - m_capacity;
				Replicator::replicate(m_bufView.ptr(), m_capacity, m_bufView.ptr(), m_currentIndex);
				m_extendedRange.reset(m_currentRange.to() - m_currentIndex, m_currentRange.to());
			}
		}

		//直接写入指定的数据，最大不超过容量。
		HX_INLINE size_t write(const T* bufPtr, size_t bufSize) HX_NOEXCEPT
		{
			if (bufSize > m_capacity)
			{//确保不超过最大块大小
				bufSize = m_capacity;
			}
			if (bufSize > 0)
			{
				if (bufSize == m_capacity)
				{//写入大小为容量大小
					Replicator::replicate(bufPtr, m_bufView.ptr(), bufSize);
					m_currentIndex = 0;
				}
				else
				{//写入大小为非容量大小
					size_t freeSize = m_capacity - m_currentIndex;
					if (bufSize > freeSize)
					{//处理前后复制需求
						Replicator::replicate(bufPtr, m_bufView.ptr(), m_currentIndex, freeSize);
						m_currentIndex = bufSize - freeSize;
						Replicator::replicate(bufPtr, freeSize, m_bufView.ptr(), m_currentIndex);
					}
					else
					{//处理一次性复制需求
						Replicator::replicate(bufPtr, m_bufView.ptr(), m_currentIndex, bufSize);
						if (bufSize == freeSize)
						{
							m_currentIndex = 0;
						}
						else
						{
							m_currentIndex += bufSize;
						}
					}
				}
				//更新当前数据范围
				if (m_currentRange.to() < m_capacity)
				{//小于容量，则直接延长
					m_currentRange += bufSize;
					if (m_currentRange.to() > m_capacity)
					{//容量已经满，更新起始
						m_currentRange.reset(m_currentRange.to() - m_capacity, m_currentRange.to());
					}
				}
				else
				{//直接进行平移
					m_currentRange >>= bufSize;
				}
			}
			return bufSize;
		}
		//直接写入指定的数据，最大不超过容量。
		HX_INLINE size_t write(const BufConstView<T>& bufView) HX_NOEXCEPT
		{
			return write(bufView.ptr(), bufView.size());
		}

		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> fetch(Hx64u from, size_t size) const
		{
			assert(size <= m_maxBlockSize);
			if (from < m_currentRange.from())
			{//起始超出当前范围
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> fetch range from < current range from");
			}
			auto to = from + size;
			if (to > m_currentRange.to())
			{//结束超出当前范围
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> fetch range to > current range to");
			}
			return BufConstView<T>(mapAddress(from, to, size), size);
		}
		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> fetch(const Span<Hx64u>& span) const
		{
			return fetch(span.from(), (size_t)span.size());
		}
		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> fetch(const Range<Hx64u>& range) const
		{
			if (!m_currentRange.isContaining(range))
			{//不在范围内
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> fetch range out of current range");
			}
			auto size = range.size();
			if (size > m_maxBlockSize)
			{//容量超过最大块
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> fetch size > maxBlockSize");
			}
			return BufConstView<T>(mapAddress(range.from(), range.to(), (size_t)size), size);
		}

		//清空数据并重置区间。
		HX_INLINE void flush() HX_NOEXCEPT
		{
			m_currentIndex = 0;
			m_currentRange.reset(0, 0);
			m_extendedRange.reset(0, 0);
		}

		//重置缓存为空。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			flush();
			m_bufView.reset();
			m_capacity = 0;
			m_maxBlockSize = 0;
		}
		//重置缓存为指定内容。
		HX_INLINE void reset(T* bufPtr, size_t capacity, size_t maxBlockSize) HX_NOEXCEPT
		{
			assert(maxBlockSize <= capacity);
			flush();
			m_bufView.reset(bufPtr, capacity + maxBlockSize);
			m_capacity = capacity;
			m_maxBlockSize = maxBlockSize;
		}
		//重置缓存为指定内容。
		HX_INLINE void reset(const BufView<T>& bufView, size_t capacity, size_t maxBlockSize) HX_NOEXCEPT
		{
			assert(maxBlockSize <= capacity);
			assert(bufView.size() >= capacity + maxBlockSize);
			flush();
			m_bufView.reset(bufView);
			m_capacity = capacity;
			m_maxBlockSize = maxBlockSize;
		}

		//确认指定的修改数据块，若有修改必须被调用，否则可能出现新数据丢失的问题。
		HX_INLINE void confirm(const BufView<T>& bufView) HX_NOEXCEPT
		{
			assert(bufView.size() <= m_maxBlockSize);
			if (bufView.size() > 0)
			{
				assert(bufView.ptr() >= m_bufView.ptr());//修改内容越界断言
				//扩展缓存地址
				auto extBufPtr = m_bufView.ptr() + m_capacity;
				//修改缓存结束地址
				auto bufViewEndPtr = bufView.ptr() + bufView.size();
				//仅修改缓存涉及到扩展缓存区域，才进行后续处理
				if (bufViewEndPtr > extBufPtr)
				{
					assert(bufViewEndPtr <= extBufPtr + m_maxBlockSize);//修改内容越界断言
					//计算修改缓存结束地址
					if (bufView.ptr() > extBufPtr)
					{//确认全部在扩展区间内
						Replicator::replicate(bufView.ptr(), m_bufView.ptr(), bufView.ptr() - extBufPtr, bufView.size());
					}
					else
					{//可能部分在扩展区间内
						Replicator::replicate(extBufPtr, m_bufView.ptr(), bufViewEndPtr - extBufPtr);
					}
				}
			}
		}

		//取出指定区间的数据块，可以执行读写操作，最大不能超过最大块大小，如果修改必须调用confirm函数。
		HX_INLINE BufView<T> getBlock(Hx64u from, size_t size)
		{
			assert(size <= m_maxBlockSize);
			if (from < m_currentRange.from())
			{//起始超出当前范围
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> getBlock range from < current range from");
			}
			auto to = from + size;
			if (to > m_currentRange.to())
			{//结束超出当前范围
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> getBlock range to > current range to");
			}
			return BufView<T>(mapAddress(from, to, size), size);
		}
		//取出指定区间的数据块，可以执行读写操作，最大不能超过最大块大小，如果修改必须调用confirm函数。
		HX_INLINE BufView<T> getBlock(const Span<Hx64u>& span)
		{
			return getBlock(span.from(), (size_t)span.size());
		}
		//取出指定区间的数据块，可以执行读写操作，最大不能超过最大块大小，如果修改必须调用confirm函数。
		HX_INLINE BufView<T> getBlock(const Range<Hx64u>& range)
		{
			if (!m_currentRange.isContaining(range))
			{//不在范围内
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> getBlock out of range");
			}
			auto size = range.size();
			if (size > m_maxBlockSize)
			{//容量超过最大块
				throw ::std::out_of_range("HxLibs::SimpleBufferStreamingView<T> getBlock size > maxBlockSize");
			}
			return BufView<T>(mapAddress(range.from(), range.to(), (size_t)size), size);
		}

		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> getConstBlock(Hx64u from, size_t size) const
		{
			return fetch(from, size);
		}
		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> getConstBlock(const Span<Hx64u>& span) const
		{
			return fetch(span.from(), (size_t)span.size());
		}
		//取出指定区间的只读数据块，最大不能超过最大块大小。
		HX_INLINE BufConstView<T> getConstBlock(const Range<Hx64u>& range) const
		{
			return fetch(range);
		}

		//重置缓存为空。
		HX_INLINE void operator= (::std::nullptr_t) HX_NOEXCEPT
		{
			reset();
		}

		//获取内部视图（仅限高级使用）。
		HX_INLINE explicit operator BufView<T>& () HX_NOEXCEPT
		{
			return m_bufView;
		}
		//获取内部视图（仅限高级使用）。
		HX_INLINE explicit operator const BufView<T>& () const HX_NOEXCEPT
		{
			return m_bufView;
		}
	};
}

#endif //!HX_SIMPLE_BUFFER_STREAMING_VIEW
#endif //__cplusplus

#endif //!HX_CORE_SIMPLE_BUFFER_STREAMING_VIEW_H
