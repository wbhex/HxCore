/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_ATOMIC_LOCKER_H
#define HX_CORE_ATOMIC_LOCKER_H

#ifdef __cplusplus
#ifndef HX_ATOMIC_LOCKER
#define HX_ATOMIC_LOCKER

#ifndef HX_ATOMIC_TRYCOUNT
#define HX_ATOMIC_TRYCOUNT 256
#endif //!HX_ATOMIC_TRYCOUNT

#include <atomic>
#include <thread>
#include <assert.h>
#include "Uncopyable.h"
#include "ThreadUtils.h"

namespace HxLibs
{
	//表示简单原子同步锁
	struct SimpleAtomicLocker : Uncopyable
	{
	private:
		//原子值
		::std::atomic<int> m_v{ 0 };

	public:
		//初始化当前对象。
		HX_INLINE SimpleAtomicLocker() HX_NOEXCEPT
		{
			//NOTHING
		}

		//锁定对象
		HX_INLINE void lock() HX_NOEXCEPT
		{
			int value = 0, counter = 0;
			while (!m_v.compare_exchange_strong(value, 1))
			{
				value = 0;
				++counter;
				if (counter >= HX_ATOMIC_TRYCOUNT)
				{
					counter = 0;
					::std::this_thread::yield();
				}
				else
				{
					HxThread_mmPause();
				}
			}
		}
		//尝试锁定对象
		HX_INLINE bool tryLock() HX_NOEXCEPT
		{
			int value = 0;
			return m_v.compare_exchange_strong(value, 1);
		}
		//尝试锁定对象，tryCount为尝试次数
		HX_INLINE bool tryLock(int tryCount) HX_NOEXCEPT
		{
			int value = 0, counter = 0;
			while (!m_v.compare_exchange_strong(value, 1))
			{
				++counter;
				if (counter >= tryCount)
				{
					return false;
				}
				value = 0;
				HxThread_mmPause();
			}
			return true;
		}
		//尝试锁定对象
		HX_INLINE bool try_lock() HX_NOEXCEPT
		{
			return tryLock();
		}
		//以自旋方式锁定对象
		HX_INLINE void spinLock() HX_NOEXCEPT
		{
			int value = 0;
			while (!m_v.compare_exchange_strong(value, 1))
			{
				value = 0;
				HxThread_mmPause();
			}
		}
		//解锁对象
		HX_INLINE void unlock() HX_NOEXCEPT
		{
			assert(m_v.load() == 1);//锁定断言
			m_v.store(0);
		}
	};

	//表示安全原子同步锁
	template <bool RECURSIVE = false>
	struct SecuredAtomicLocker {};

	//表示安全原子同步锁基类
	struct __SecuredAtomicLocker__ : Uncopyable
	{
	private:
		//表示线程ID
		union ThreadID
		{
			size_t val1;
			::std::thread::id val2;
		};

	protected:
		//原子值
		::std::atomic<size_t> m_v{ 0 };

		//获取当前线程ID。
		HX_INLINE static size_t currentThreadId() HX_NOEXCEPT
		{
			ThreadID id{ 0 };
			id.val2 = ::std::this_thread::get_id();
			return id.val1;
		}

		//初始化当前对象。
		HX_INLINE __SecuredAtomicLocker__() HX_NOEXCEPT
		{
			//NOTHING
		}

	public:
		//获取当前锁定线程ID。
		HX_INLINE::std::thread::id lockedThreadId() const HX_NOEXCEPT
		{
			ThreadID id{ m_v.load() };
			return id.val2;
		}
	};
	//可递归原子同步锁实现
	template <>
	struct SecuredAtomicLocker<true> : __SecuredAtomicLocker__
	{
	private:
		//计数器
		size_t m_cnt{ 0 };

	public:
		//锁定对象
		HX_INLINE void lock() HX_NOEXCEPT
		{
			size_t value = 0, counter = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					m_cnt = 1;
					return;
				}
				else if (value == threadId)
				{
					++m_cnt;
					return;
				}
				else
				{
					value = 0;
					++counter;
					if (counter >= HX_ATOMIC_TRYCOUNT)
					{
						counter = 0;
						::std::this_thread::yield();
					}
					else
					{
						HxThread_mmPause();
					}
				}
			}
		}
		//尝试锁定对象
		HX_INLINE bool tryLock() HX_NOEXCEPT
		{
			size_t value = 0;
			const auto threadId = currentThreadId();
			auto result = m_v.compare_exchange_strong(value, threadId);
			if (result)
			{
				m_cnt = 1;
				return true;
			}
			else if (value == threadId)
			{
				++m_cnt;
				return true;
			}
			return false;
		}
		//尝试锁定对象，tryCount为尝试次数
		HX_INLINE bool tryLock(int tryCount) HX_NOEXCEPT
		{
			size_t value = 0, counter = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					m_cnt = 1;
					return true;
				}
				else if (value == threadId)
				{
					++m_cnt;
					return true;
				}
				else
				{
					++counter;
					if (counter >= tryCount)
					{
						return false;
					}
					value = 0;
					HxThread_mmPause();
				}
			}
		}
		//尝试锁定对象
		HX_INLINE bool try_lock() HX_NOEXCEPT
		{
			return tryLock();
		}
		//以自旋方式锁定对象
		HX_INLINE void spinLock() HX_NOEXCEPT
		{
			size_t value = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					m_cnt = 1;
					return;
				}
				else if (value == threadId)
				{
					++m_cnt;
					return;
				}
				else
				{
					value = 0;
					HxThread_mmPause();
				}
			}
		}
		//解锁对象
		HX_INLINE void unlock() HX_NOEXCEPT
		{
			const auto threadId = currentThreadId();
			if (m_v.load() == threadId)
			{
				--m_cnt;
				if (m_cnt == 0)
				{
					m_v.store(0);
				}
			}
		}
	};
	//不可递归原子同步锁实现
	template <>
	struct SecuredAtomicLocker<false> : __SecuredAtomicLocker__
	{
	public:
		//锁定对象
		HX_INLINE void lock() HX_NOEXCEPT
		{
			size_t value = 0, counter = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					return;
				}
				assert(value != threadId);//重入断言
				value = 0;
				++counter;
				if (counter >= HX_ATOMIC_TRYCOUNT)
				{
					counter = 0;
					::std::this_thread::yield();
				}
				else
				{
					HxThread_mmPause();
				}
			}
		}
		//尝试锁定对象
		HX_INLINE bool tryLock() HX_NOEXCEPT
		{
			size_t value = 0;
			const auto threadId = currentThreadId();
			auto result = m_v.compare_exchange_strong(value, threadId);
			if (result)
			{
				return true;
			}
			assert(value != threadId);//重入断言
			return false;
		}
		//尝试锁定对象，tryCount为尝试次数
		HX_INLINE bool tryLock(int tryCount) HX_NOEXCEPT
		{
			size_t value = 0, counter = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					return true;
				}
				assert(value != threadId);//重入断言
				++counter;
				if (counter >= tryCount)
				{
					return false;
				}
				value = 0;
				HxThread_mmPause();
			}
		}
		//尝试锁定对象
		HX_INLINE bool try_lock() HX_NOEXCEPT
		{
			return tryLock();
		}
		//以自旋方式锁定对象
		HX_INLINE void spinLock() HX_NOEXCEPT
		{
			size_t value = 0;
			const auto threadId = currentThreadId();
			while (true)
			{
				auto result = m_v.compare_exchange_strong(value, threadId);
				if (result)
				{
					return;
				}
				assert(value != threadId);//重入断言
				value = 0;
				HxThread_mmPause();
			}
		}
		//解锁对象
		HX_INLINE void unlock() HX_NOEXCEPT
		{
			assert(currentThreadId() == m_v.load());//锁定断言
			m_v.store(0);
		}
	};

	//定义简单原子同步锁
	using AtomicLocker = SimpleAtomicLocker;
	//定义可递归原子同步锁
	using RecursiveAtomicLocker = SecuredAtomicLocker<true>;
}

#endif //!HX_ATOMIC_LOCKER
#endif //__cplusplus

#endif //!HX_CORE_ATOMIC_LOCKER_H
