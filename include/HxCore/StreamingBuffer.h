/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_STREAMING_BUFFER_H
#define HX_CORE_STREAMING_BUFFER_H

#ifdef __cplusplus
#ifndef HX_CORE_STREAMING_BUFFER
#define HX_CORE_STREAMING_BUFFER

#include "Memory.h"
#include "RefCnt.h"
#include "LockGuard.h"
#include "AtomicLocker.h"
#include "SimpleBuffer.h"
#include "MemoryFixedSizePool.h"
#include "StreamingBufferSlim.h"
#include "StreamingBufferSegment.h"

namespace HxLibs
{
	template <typename T, typename LockFlag = short, typename BufMemGlobal = MemoryGlobalDefault, typename ObjMemGlobal = MemoryGlobalDefault>
	class StreamingBuffer
	{
		using Locker = ::HxLibs::AtomicLocker;
		using LockGuard = ::HxLibs::SpinLockGuard<Locker>;

	private:
		//表示缓存实现。
		struct Impl
		{
			mutable Locker locker;
			SimpleBuffer<T, BufMemGlobal> bufData;
			SimpleBuffer<LockFlag, ObjMemGlobal> lockData;
			StreamingBufferSlim<T, LockFlag> bufferSlim;
			MemoryFixedSizePool<char, ObjMemGlobal> objBufPool;

			//初始化缓存
			HX_INLINE Impl(size_t blockSize, size_t blockCount, size_t objBufSize) HX_NOEXCEPT
				: bufData(blockSize* blockCount)
				, lockData(blockCount)
				, bufferSlim(bufData, lockData, blockSize)
				, objBufPool(objBufSize, HX_FIXEDSIZE_POOL_LIMIT_NONE, (int)blockCount)
			{
				//NOTHING
			}
		};
		//表示缓存片段。
		class Segment : public StreamingBufferSegment<T>
		{
		private:
			RefCnt m_refCnt;
			Hx64u m_from;
			::std::shared_ptr<Impl> m_owner;
			BufConstView<T> m_view1;
			BufConstView<T> m_view2;

		public:
			//初始化当前对象。
			HX_INLINE Segment(const ::std::shared_ptr<Impl>& onwer, Hx64u from, const BufConstView<T>& view) HX_NOEXCEPT
				: m_owner(onwer), m_from(from), m_view1(view)
			{
				//NOTHING
			}
			//初始化当前对象。
			HX_INLINE Segment(const ::std::shared_ptr<Impl>& onwer, Hx64u from, const BufConstView<T>& view1, const BufConstView<T>& view2) HX_NOEXCEPT
				: m_owner(onwer), m_from(from), m_view1(view1), m_view2(view2)
			{
				//NOTHING
			}
			//释放当前对象所占用的相应资源。
			~Segment()
			{
				LockGuard lck(m_owner->locker);
				m_owner->bufferSlim.unlockView(m_view1);
				if (m_view2.size() > 0)
				{
					m_owner->bufferSlim.unlockView(m_view2);
				}
			}


			virtual void HX_STDCALL addRef() HX_NOEXCEPT override
			{
				m_refCnt.addRef();
			}

			virtual bool HX_STDCALL release() HX_NOEXCEPT override
			{
				if (m_refCnt.release())
				{
					auto onwer = m_owner;
					this->~Segment();
					onwer->objBufPool.free((char*)this, sizeof(Segment));
					return true;
				}
				return false;
			}


			virtual size_t retrieve(BufConstView<T>* view1, BufConstView<T>* view2, Hx64u* from) const HX_NOEXCEPT override
			{
				if (from != nullptr)
				{
					*from = m_from;
				}
				*view1 = m_view1;
				if (view2 != nullptr)
				{
					*view2 = m_view2;
					return m_view1.size() + m_view2.size();
				}
				return m_view1.size();
			}


			virtual Hx64u HX_STDCALL lockExtraView(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2) HX_NOEXCEPT override
			{
				LockGuard lck(m_owner->locker);
				return m_owner->bufferSlim.lockView(from, size, view1, view2);
			}

			virtual size_t HX_STDCALL tryLockExtraView(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2) HX_NOEXCEPT override
			{
				LockGuard lck(m_owner->locker);
				return m_owner->bufferSlim.tryLockView(from, size, view1, view2);
			}

			virtual void HX_STDCALL unlockExtraView(BufConstView<T> view) HX_NOEXCEPT override
			{
				LockGuard lck(m_owner->locker);
				m_owner->bufferSlim.unlockView(view);
			}


			virtual StreamingBufferSegment<T>* HX_STDCALL lockExtraSegment(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2, Hx64u* fromActual) HX_NOEXCEPT override
			{
				LockGuard lck(m_owner->locker);
				BufConstView<T> view1x, view2x;
				auto fromActualx = m_owner->bufferSlim.lockView(from, size, view1x, view2x);
				if (view1 != nullptr)
				{
					*view1 = view1x;
				}
				if (view2 != nullptr)
				{
					*view2 = view2x;
				}
				if (fromActual != nullptr)
				{
					*fromActual = fromActualx;
				}
				if (view1x.size() > 0)
				{
					char* objBuf = m_owner->objBufPool.alloc();
					return new (objBuf) Segment(m_owner, fromActualx, view1x, view2x);
				}
				return nullptr;
			}

			virtual StreamingBufferSegment<T>* HX_STDCALL tryLockExtraSegment(Hx64u from, size_t size, BufConstView<T>* view1, BufConstView<T>* view2) HX_NOEXCEPT override
			{
				LockGuard lck(m_owner->locker);
				BufConstView<T> view1x, view2x;
				m_owner->bufferSlim.tryLockView(from, size, view1x, view2x);
				if (view1 != nullptr)
				{
					*view1 = view1x;
				}
				if (view2 != nullptr)
				{
					*view2 = view2x;
				}
				if (view1x.size() > 0)
				{
					char* objBuf = m_owner->objBufPool.alloc();
					return new (objBuf) Segment(m_owner, from, view1x, view2x);
				}
				return nullptr;
			}
		};

		//缓存
		::std::shared_ptr<Impl> m_impl;

	public:
		//初始化当前对象
		HX_INLINE StreamingBuffer() HX_NOEXCEPT
		{
			//NOTHING
		}
		//初始化当前对象
		HX_INLINE StreamingBuffer(size_t blockSize, size_t blockCount) HX_NOEXCEPT
			: m_impl(Memory::makeSharedObject<Impl>(blockSize, blockCount, sizeof(Segment)))
		{
			//NOTHING
		}
		//释放当前对象所占用的相应资源
		~StreamingBuffer()
		{
			//NOTHING
		}

		//移动构造函数
		HX_INLINE StreamingBuffer(StreamingBuffer&& other) HX_NOEXCEPT
			: m_impl(::std::move(other.m_impl))
		{
			//NOTHING
		}
		//移动赋值操作符
		HX_INLINE StreamingBuffer& operator=(StreamingBuffer&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_impl = ::std::move(other.m_impl);
			}
			return *this;
		}

		//拷贝构造函数
		HX_INLINE StreamingBuffer(const StreamingBuffer& other) HX_NOEXCEPT
			: m_impl(other.m_impl)
		{
			//NOTHING
		}
		//拷贝赋值操作符
		HX_INLINE StreamingBuffer& operator=(const StreamingBuffer& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_impl = other.m_impl;
			}
			return *this;
		}

		//拷贝赋值操作符
		HX_INLINE StreamingBuffer& operator=(::std::nullptr_t) HX_NOEXCEPT
		{
			m_impl.reset();
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(StreamingBuffer& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				m_impl.swap(other.m_impl);
			}
		}

		//重置缓存。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			m_impl.reset();
		}
		HX_INLINE void reset(size_t blockSize, size_t blockCount) HX_NOEXCEPT
		{
			m_impl = Memory::makeSharedObject<Impl>(blockSize, blockCount, sizeof(Segment));
		}

		//获取缓存容量。
		HX_INLINE size_t capacity() const HX_NOEXCEPT
		{
			if (m_impl)
			{
				return m_impl->bufferSlim.capacity();
			}
			return 0;
		}
		//获取分块大小。
		HX_INLINE size_t blockSize() const HX_NOEXCEPT
		{
			if (m_impl)
			{
				return m_impl->bufferSlim.blockSize();
			}
			return 0;
		}
		//获取分块数量。
		HX_INLINE size_t blockCount() const HX_NOEXCEPT
		{
			if (m_impl)
			{
				return m_impl->bufferSlim.blockCount();
			}
			return 0;
		}

		//获取可用大小。
		HX_INLINE size_t availableSize() const HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				return m_impl->bufferSlim.availableSize();
			}
			return 0;
		}

		//将指定的写入数据应用到缓存中并返回总共数据量（应用的缓存必须是之前申请的缓存，大小可以小于申请缓存大小）。
		HX_INLINE Hx64u commit(const BufView<T>& view) HX_NOEXCEPT
		{
			LockGuard lck(m_impl->locker);
			return m_impl->bufferSlim.commit(view);
		}
		//申请指定大小的缓存片段以进行写入，如果可写空间在尾部且跨越头部，则将仅返回尾部区间视图，如果返回长度为0表示当前无可写空间。
		HX_INLINE BufView<T> allocate(size_t size, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				return m_impl->bufferSlim.allocate(size, from);
			}
			else
			{
				if (from != nullptr)
				{
					*from = 0;
				}
				return BufView<T>{};
			}
		}
		HX_INLINE BufView<T> allocate(size_t size, Hx64u& from) HX_NOEXCEPT
		{
			return allocate(size, &from);
		}

		//锁定指定区间的缓存视图以进行读取并返回实际的区间起始，如果指定区间不能锁定将会缩小锁定的区间范围：
		//	如果缓存区间在缓存尾部和首部，view2将为首部区间视图，否则为空;
		//	如果view2参数为空，则锁定的大小不会包含首部区间视图。
		HX_INLINE Hx64u lockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				return m_impl->bufferSlim.lockView(from, size, view1, view2);
			}
			else
			{
				view1.reset(nullptr, 0);
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				return 0;
			}
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
		//	如果缓存区间在缓存尾部和首部，view2将为首部区间视图，否则为空。
		//	如果view2参数为空且锁定的区间包含首部区间时将不会锁定该首部区间，返回大小只包含缓存尾部区间。
		HX_INLINE size_t tryLockView(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				return m_impl->bufferSlim.tryLockView(from, size, view1, view2);
			}
			else
			{
				view1.reset(nullptr, 0);
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				return 0;
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
		{
			LockGuard lck(m_impl->locker);
			m_impl->bufferSlim.unlockView(view);
		}

		//锁定指定区间的缓存片段以进行读取，如果指定区间不能锁定将会缩小锁定的区间范围。
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, BufConstView<T>* view1 = nullptr, BufConstView<T>* view2 = nullptr, Hx64u* fromActual = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				BufConstView<T> view1x, view2x;
				auto fromActualx = m_impl->bufferSlim.lockView(from, size, view1x, view2x);
				if (view1 != nullptr)
				{
					*view1 = view1x;
				}
				if (view2 != nullptr)
				{
					*view2 = view2x;
				}
				if (fromActual != nullptr)
				{
					*fromActual = fromActualx;
				}
				if (view1x.size() > 0)
				{
					char* objBuf = m_impl->objBufPool.alloc();
					return new (objBuf) Segment(m_impl, fromActualx, view1x, view2x);
				}
				return nullptr;
			}
			else
			{
				if (view1 != nullptr)
				{
					view1->reset(nullptr, 0);
				}
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				if (fromActual != nullptr)
				{
					*fromActual = 0;
				}
				return nullptr;
			}
		}
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, Hx64u& fromActual) HX_NOEXCEPT
		{
			return lockSegment(from, size, nullptr, nullptr, &fromActual);
		}
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, BufConstView<T>& view1) HX_NOEXCEPT
		{
			return lockSegment(from, size, &view1, nullptr, nullptr);
		}
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, BufConstView<T>& view1, Hx64u& fromActual) HX_NOEXCEPT
		{
			return lockSegment(from, size, &view1, nullptr, &fromActual);
		}
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>& view2) HX_NOEXCEPT
		{
			return lockSegment(from, size, &view1, &view2, nullptr);
		}
		HX_INLINE StreamingBufferSegment<T>* lockSegment(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>& view2, Hx64u& fromActual) HX_NOEXCEPT
		{
			return lockSegment(from, size, &view1, &view2, &fromActual);
		}
		//尝试锁定的指定区间的缓存片段以进行读取，如果指定的区间不满足读取条件将返回NULL。
		HX_INLINE StreamingBufferSegment<T>* tryLockSegment(Hx64u from, size_t size, BufConstView<T>* view1 = nullptr, BufConstView<T>* view2 = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				BufConstView<T> view1x, view2x;
				m_impl->bufferSlim.tryLockView(from, size, view1x, view2x);
				if (view1 != nullptr)
				{
					*view1 = view1x;
				}
				if (view2 != nullptr)
				{
					*view2 = view2x;
				}
				if (view1x.size() > 0)
				{
					char* objBuf = m_impl->objBufPool.alloc();
					return new (objBuf) Segment(m_impl, from, view1x, view2x);
				}
				return nullptr;
			}
			else
			{
				if (view1 != nullptr)
				{
					view1->reset(nullptr, 0);
				}
				if (view2 != nullptr)
				{
					view2->reset(nullptr, 0);
				}
				return nullptr;
			}
		}
		HX_INLINE StreamingBufferSegment<T>* tryLockSegment(Hx64u from, size_t size, BufConstView<T>& view1) HX_NOEXCEPT
		{
			return tryLockSegment(from, size, &view1, nullptr);
		}
		HX_INLINE StreamingBufferSegment<T>* tryLockSegment(Hx64u from, size_t size, BufConstView<T>& view1, BufConstView<T>& view2) HX_NOEXCEPT
		{
			return tryLockSegment(from, size, &view1, &view2);
		}

		//查询缓存当前信息。
		HX_INLINE void queryInfo(size_t* capacity, size_t* blockSize) const HX_NOEXCEPT
		{
			if (m_impl)
			{
				if (capacity != nullptr)
				{
					*capacity = m_impl->bufferSlim.capacity();
				}
				if (blockSize != nullptr)
				{
					*blockSize = m_impl->bufferSlim.blockSize();
				}
			}
			else
			{
				if (capacity != nullptr)
				{
					*capacity = 0;
				}
				if (blockSize != nullptr)
				{
					*blockSize = 0;
				}
			}
		}
		HX_INLINE void queryInfo(size_t& capacity, size_t& blockSize) const HX_NOEXCEPT
		{
			if (m_impl)
			{
				capacity = m_impl->bufferSlim.capacity();
				blockSize = m_impl->bufferSlim.blockSize();
			}
			else
			{
				capacity = 0;
				blockSize = 0;
			}
		}
		//查询缓存当前状态。
		HX_INLINE void queryState(StreamingBufferState& value) const HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				m_impl->bufferSlim.queryState(value);
			}
			else
			{
				value.capacity = 0;
				value.blockSize = 0;
				value.validFrom = 0;
				value.validSize = 0;
				value.availableSize = 0;
			}
		}
		HX_INLINE void queryState(StreamingBufferState* value) const HX_NOEXCEPT
		{
			return queryState(*value);
		}

		//申请指定数量的缓存块以进行写入，如果可写空间在尾部且跨越头部，则将仅返回尾部区间视图，如果返回长度为0表示当前无可写空间。
		HX_INLINE BufView<T> allocBlocks(size_t count = 1, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			if (m_impl)
			{
				LockGuard lck(m_impl->locker);
				return m_impl->bufferSlim.allocBlocks(count, from);
			}
			else
			{
				if (from != nullptr)
				{
					*from = 0;
				}
				return BufView<T>{};
			}
		}
		HX_INLINE BufView<T> allocBlocks(size_t count, Hx64u& from) HX_NOEXCEPT
		{
			return allocBlocks(count, &from);
		}
		//将指定的写入数据应用到缓存中并锁定该缓存区域视图（应用的缓存必须是之前申请的缓存，大小可以小于申请缓存大小）。
		HX_INLINE BufConstView<T> commitLockView(const BufView<T>& view, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(m_impl->locker);
			return m_impl->bufferSlim.commitLockView(view, from);
		}
		HX_INLINE BufConstView<T> commitLockView(const BufView<T>& view, Hx64u& from) HX_NOEXCEPT
		{
			return commitLockView(view, &from);
		}

		HX_INLINE StreamingBufferSegment<T>* commitLockSegment(const BufView<T>& view, Hx64u* from = nullptr) HX_NOEXCEPT
		{
			LockGuard lck(m_impl->locker);
			Hx64u fromActual;
			auto outView = m_impl->bufferSlim.commitLockView(view, fromActual);
			if (from != nullptr)
			{
				*from = fromActual;
			}
			if (outView.size() > 0)
			{
				char* objBuf = m_impl->objBufPool.alloc();
				return new (objBuf) Segment(m_impl, fromActual, outView);
			}
			return nullptr;
		}
		HX_INLINE StreamingBufferSegment<T>* commitLockSegment(const BufView<T>& view, Hx64u& from) HX_NOEXCEPT
		{
			return commitLockSegment(view, &from);
		}

		//清除之前的申请记录，后续从写入头开始重新申请缓存。
		HX_INLINE void reallocate() HX_NOEXCEPT
		{
			LockGuard lck(m_impl->locker);
			m_impl->bufferSlim.reallocate();
		}
	};
}
template <typename T, typename LockFlag = short, typename BufMemGlobal = HxMemoryGlobalDefault, typename ObjMemGlobal = HxMemoryGlobalDefault>
using HxStreamingBuffer = ::HxLibs::StreamingBuffer<T, LockFlag, BufMemGlobal, ObjMemGlobal>;

#endif //!HX_CORE_STREAMING_BUFFER
#endif //__cplusplus

#endif //!HX_CORE_STREAMING_BUFFER_H
