/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明，未经同意不得公开或提供给第三方使用。
 */

#pragma once
#ifndef HX_CORE_WORK_LOOP_H
#define HX_CORE_WORK_LOOP_H

#ifdef __cplusplus
#ifndef HX_WORK_LOOP
#define HX_WORK_LOOP

#include <functional>
#include <condition_variable>

#include "CString.h"
#include "LockGuard.h"
#include "ThreadUtils.h"
#include "Containers/Vector.h"

namespace HxLibs
{
	//表示工作项目。
	using WorkItem = ::std::function<void()>;
	//表示工作循环。
	class WorkLoop : private Uncopyable
	{
		using WorkList = ::HxLibs::Containers::Vector<WorkItem>;
		using LockGuard = ::HxLibs::SpinLockGuard<::std::mutex>;

	private:
		String m_name;
		volatile bool m_status{ true };
		volatile bool m_running{ true };
		volatile bool m_suspend{ false };
		::std::condition_variable m_cond;
		::std::thread m_thread;
		::std::mutex m_mutex;
		WorkList m_queue;
		WorkList m_threadQueue;
		::std::atomic<size_t> m_executing{ 0 };
		::std::atomic<unsigned long long> m_executed{ 0 };

		//交换工作队列。
		HX_INLINE void swap(WorkList& queue) HX_NOEXCEPT
		{
			::std::unique_lock<::std::mutex> lck(m_mutex);
			while (m_queue.size() == 0)
			{
				m_suspend = true;
				m_cond.wait(lck);
				m_suspend = false;
			}
			m_queue.swap(queue);
		}
		//执行指定队列。
		HX_INLINE void exec(WorkList& queue) HX_NOEXCEPT
		{
			for (auto& entry : queue)
			{
				entry();
				entry = nullptr;
				m_executed.fetch_add(1, ::std::memory_order_relaxed);
				m_executing.fetch_sub(1, ::std::memory_order_relaxed);
			}
			queue.clear();
		}

		//队列初始大小。
		static HX_CONSTEXPR size_t INIT_COUNT = 256;
		//线程处理函数。
		void main() HX_NOEXCEPT
		{
			if (!m_name.empty())
			{
				HxThread_setName(m_name.c_str(), m_name.size());
			}
			while (m_running)
			{
				swap(m_threadQueue);
				exec(m_threadQueue);
			}
		}

	public:
		//获取名称。
		HX_INLINE const String& name() const HX_NOEXCEPT
		{
			return m_name;
		}

		//获取线程对象
		HX_INLINE::std::thread& thread() HX_NOEXCEPT
		{
			return m_thread;
		}
		//获取线程对象
		HX_INLINE const ::std::thread& thread() const HX_NOEXCEPT
		{
			return m_thread;
		}

		//已执行数量。
		HX_INLINE unsigned long long executed() const HX_NOEXCEPT
		{
			return m_executed.load(::std::memory_order_relaxed);
		}
		//待执行数量。
		HX_INLINE size_t executing() const HX_NOEXCEPT
		{
			return m_executing.load(::std::memory_order_relaxed);
		}

		//添加一个工作项。
		HX_INLINE bool push(WorkItem&& item) HX_NOEXCEPT
		{
			if (m_status)
			{
				LockGuard lck(m_mutex);
				if (m_status)
				{
					m_executing.fetch_add(1, ::std::memory_order_relaxed);
					m_queue.emplace_back(::std::move(item));
					if (m_suspend)
					{
						m_cond.notify_one();
					}
					return true;
				}
			}
			return false;
		}
		//添加一个工作项。
		HX_INLINE bool push(const WorkItem& item) HX_NOEXCEPT
		{
			if (m_status)
			{
				LockGuard lck(m_mutex);
				if (m_status)
				{
					m_executing.fetch_add(1, ::std::memory_order_relaxed);
					m_queue.emplace_back(item);
					if (m_suspend)
					{
						m_cond.notify_one();
					}
					return true;
				}
			}
			return false;
		}

		//发送退出命令。
		HX_INLINE bool exit() HX_NOEXCEPT
		{
			if (m_status)
			{
				LockGuard lck(m_mutex);
				if (m_status)
				{
					m_status = false;
					m_executing.fetch_add(1, ::std::memory_order_relaxed);
					m_queue.emplace_back([this]()
						{
							m_running = false;
						});
					m_cond.notify_one();
					return true;
				}
			}
			return false;
		}
		//等待退出完成。
		HX_INLINE bool join() HX_NOEXCEPT
		{
			if (m_thread.joinable())
			{
				m_thread.join();
				return true;
			}
			return false;
		}

		//初始化当前对象。
		HX_INLINE explicit WorkLoop(const CString& name = CString{}) HX_NOEXCEPT
			: m_name(name.ptr(), name.size())
		{
			m_queue.reserve(INIT_COUNT);
			m_threadQueue.reserve(INIT_COUNT);
			m_thread = ::std::thread(&WorkLoop::main, this);
		}

		//释放当前对象所占用的相关资源。
		HX_INLINE ~WorkLoop() HX_NOEXCEPT
		{
			exit();
			join();
		}
	};
}

#endif //!HX_WORK_LOOP
#endif //__cplusplus

#endif //!HX_CORE_WORK_LOOP_H
