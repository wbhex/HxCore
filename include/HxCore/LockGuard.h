/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_LOCKGUARD_H
#define HX_CORE_LOCKGUARD_H

#ifdef __cplusplus
#ifndef HX_LOCKGUARD
#define HX_LOCKGUARD

#include <mutex>
#include "AtomicLocker.h"

namespace HxLibs
{
	//循环尝试锁定。
	template <typename Locker>
	HX_INLINE static void __loopLock__(Locker& locker) HX_NOEXCEPT
	{
		while (!locker.try_lock())
		{
			HxThread_mmPause();
		}
	}
	//循环尝试锁定，如果超出指定次数则切换线程。
	template <typename Locker>
	HX_INLINE static void __loopLock__(Locker& locker, int tryCount) HX_NOEXCEPT
	{
		int counter = 0;
		while (!locker.try_lock())
		{
			++counter;
			if (counter >= tryCount)
			{//超过尝试次数，让出执行权限
				counter = 0;
				::std::this_thread::yield();
			}
			else
			{
				HxThread_mmPause();
			}
		}
	}

	//循环尝试锁定。
	HX_INLINE static void __loopLock__(SimpleAtomicLocker& locker) HX_NOEXCEPT
	{
		locker.spinLock();
	}
	//循环尝试锁定，如果超出指定次数则切换线程。
	HX_INLINE static void __loopLock__(SimpleAtomicLocker& locker, int tryCount) HX_NOEXCEPT
	{
		while (!locker.tryLock(tryCount))
		{
			::std::this_thread::yield();
		}
	}

	//循环尝试锁定。
	template <bool RECURSIVE>
	HX_INLINE static void __loopLock__(SecuredAtomicLocker<RECURSIVE>& locker) HX_NOEXCEPT
	{
		locker.spinLock();
	}
	//循环尝试锁定，如果超出指定次数则切换线程。
	template <bool RECURSIVE>
	HX_INLINE static void __loopLock__(SecuredAtomicLocker<RECURSIVE>& locker, int tryCount) HX_NOEXCEPT
	{
		while (!locker.tryLock(tryCount))
		{
			::std::this_thread::yield();
		}
	}

	//表示锁定。
	template <typename Locker>
	struct LockGuard : Uncopyable
	{
	private:
		//锁定标识
		bool m_locked;
		//锁对象引用
		Locker& m_locker;

	public:
		//获取锁
		HX_INLINE void lock() HX_NOEXCEPT
		{
			if (!m_locked)
			{
				m_locker.lock();
				m_locked = true;
			}
		}
		//获取锁，如果tryCount＞0则会尝试指定次数，超出该次数则会切换线程
		HX_INLINE void lock(int tryCount) HX_NOEXCEPT
		{
			if (!m_locked)
			{
				if (tryCount > 0)
				{
					__loopLock__(m_locker, tryCount);
				}
				else
				{
					m_locker.lock();
				}
				m_locked = true;
			}
		}
		//释放锁
		HX_INLINE void unlock() HX_NOEXCEPT
		{
			if (m_locked)
			{
				m_locked = false;
				m_locker.unlock();
			}
		}

		//获取锁
		HX_INLINE explicit LockGuard(Locker& locker) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(true)
		{
			m_locker.lock();
		}
		//获取锁，如果tryCount＞0则会尝试指定次数，超出该次数则会切换线程
		HX_INLINE LockGuard(Locker& locker, int tryCount) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(true)
		{
			if (tryCount > 0)
			{
				__loopLock__(m_locker, tryCount);
			}
			else
			{
				m_locker.lock();
			}
		}
		//初始化对象，但不获取锁
		HX_INLINE LockGuard(Locker& locker, ::std::defer_lock_t) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(false)
		{
			//NOTHING
		}

		//释放锁
		HX_INLINE ~LockGuard() HX_NOEXCEPT
		{
			unlock();
		}
	};

	//表示自旋锁定。
	template <typename Locker>
	struct SpinLockGuard : Uncopyable
	{
	private:
		//锁定标识
		bool m_locked;
		//锁对象引用
		Locker& m_locker;

	public:
		//获取锁
		HX_INLINE void lock() HX_NOEXCEPT
		{
			if (!m_locked)
			{
				__loopLock__(m_locker);
				m_locked = true;
			}
		}
		//获取锁，如果tryCount＞0则会尝试指定次数，超出该次数则会切换线程
		HX_INLINE void lock(int tryCount) HX_NOEXCEPT
		{
			if (!m_locked)
			{
				if (tryCount > 0)
				{
					__loopLock__(m_locker, tryCount);
				}
				else
				{
					__loopLock__(m_locker);
				}
				m_locked = true;
			}
		}
		//释放锁
		HX_INLINE void unlock() HX_NOEXCEPT
		{
			if (m_locked)
			{
				m_locked = false;
				m_locker.unlock();
			}
		}

		//获取锁
		HX_INLINE explicit SpinLockGuard(Locker& locker) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(true)
		{
			__loopLock__(m_locker);
		}
		//获取锁，如果tryCount＞0则会尝试指定次数，超出该次数则会切换线程
		HX_INLINE SpinLockGuard(Locker& locker, int tryCount) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(true)
		{
			if (tryCount > 0)
			{
				__loopLock__(m_locker, tryCount);
			}
			else
			{
				__loopLock__(m_locker);
			}
		}
		//初始化对象，但不获取锁
		HX_INLINE SpinLockGuard(Locker& locker, ::std::defer_lock_t) HX_NOEXCEPT
			: m_locker(locker)
			, m_locked(false)
		{
			//NOTHING
		}

		//释放锁
		HX_INLINE ~SpinLockGuard() HX_NOEXCEPT
		{
			unlock();
		}
	};
}

#endif //!HX_LOCKGUARD
#endif //__cplusplus

#endif //!HX_CORE_LOCKGUARD_H
