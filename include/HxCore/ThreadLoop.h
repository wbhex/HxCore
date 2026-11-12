#pragma once
#ifndef HX_CORE_THREAD_LOOP_H
#define HX_CORE_THREAD_LOOP_H

#ifdef __cplusplus
#ifndef HX_THREAD_LOOP
#define HX_THREAD_LOOP

#include <condition_variable>

#include "CString.h"
#include "LockGuard.h"
#include "SteadyClock.h"
#include "ThreadUtils.h"
#include "ThreadTaskItem.h"
#include "Containers/Vector.h"

namespace HxLibs
{
	struct ThreadLoop : Uncopyable
	{
		using LockGuard = ::HxLibs::SpinLockGuard<::std::mutex>;

	private:
		//名称
		String m_name;
		//同步
		std::mutex m_mutex;
		std::condition_variable m_cond;
		//队列
		struct Entry
		{
			ThreadTaskItem* item;
			SteadyClock::TimePoint time;

			Entry(const Entry&) = delete;
			Entry& operator=(const Entry&) = delete;

			HX_INLINE Entry() HX_NOEXCEPT
				: item(nullptr)
			{
				//NOTHING
			}
			HX_INLINE explicit Entry(ThreadTaskItem* item) HX_NOEXCEPT
				: item(item)
				, time(SteadyClock::now())
			{
				//NOTHING
			}
			~Entry()
			{
				if (item != nullptr)
				{
					item->release();
					item = nullptr;
				}
			}

			HX_INLINE Entry(Entry&& other) HX_NOEXCEPT
				: item(other.item)
				, time(other.time)
			{
				other.item = nullptr;
			}
			HX_INLINE Entry& operator=(Entry&& other) HX_NOEXCEPT
			{
				if (this != &other)
				{
					if (item != nullptr)
					{
						item->release();
					}
					item = other.item;
					time = other.time;
					other.item = nullptr;
				}
				return *this;
			}

			HX_INLINE void execute() HX_NOEXCEPT
			{
				item->execute();
			}

			HX_INLINE void release() HX_NOEXCEPT
			{
				item->release();
				item = nullptr;
			}
		};
		Vector<Entry> m_queue;
		Vector<Entry> m_threadQueue;
		//线程
		std::thread m_thread;
		volatile bool m_running{ false };
		volatile bool m_suspend{ false };
		//统计
		::std::atomic<size_t> m_executing{ 0 };
		::std::atomic<unsigned long long> m_executed{ 0 };

		//禁用复制构造函数和复制赋值操作符
		ThreadLoop(const ThreadLoop&) = delete;
		ThreadLoop& operator=(const ThreadLoop&) = delete;

		//交换工作队列。
		HX_INLINE void swap(Vector<Entry>& queue) HX_NOEXCEPT
		{
			::std::unique_lock<::std::mutex> lck(m_mutex);
			while (m_queue.size() == 0 && m_running)
			{
				m_suspend = true;
				m_cond.wait(lck);
				m_suspend = false;
			}
			m_queue.swap(queue);
		}

		//队列初始大小。
		static HX_CONSTEXPR size_t INIT_COUNT = 256;
		//线程处理函数
		void main() HX_NOEXCEPT
		{
			if (m_name.empty())
			{
				HxThread_setName(m_name.c_str(), m_name.size());
			}

			while (m_running)
			{
				swap(m_threadQueue);
				for (auto& entry : m_threadQueue)
				{
					if (m_running)
					{
						entry.execute();
						m_executed.fetch_add(1, std::memory_order_relaxed);
					}
					entry.release();
					m_executing.fetch_sub(1, std::memory_order_relaxed);
				}
				m_threadQueue.clear();
			}
		}

	public:
		//获取线程名称
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

		//退出处理
		HX_INLINE void exit() HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			if (m_running)
			{
				m_running = false;
				m_queue.clear();
				m_cond.notify_one();
			}
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

		//初始化并启动处理
		HX_INLINE void init() HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			if (!m_running)
			{
				m_running = true;
				m_executing = 0;
				m_executed = 0;
				m_queue.reserve(INIT_COUNT);
				m_threadQueue.reserve(INIT_COUNT);
				m_thread = ::std::thread(&ThreadLoop::main, this);
			}
		}

		//提交待处理任务条目
		HX_INLINE bool submit(ThreadTaskItem* item) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			if (m_running)
			{
				m_executing.fetch_add(1, std::memory_order_relaxed);
				m_queue.emplace_back(item);
				if (m_suspend)
				{
					m_cond.notify_one();
				}
				return true;
			}
			item->release();
			return false;
		}
		//提交待处理任务条目，并返回一个时间戳。
		HX_INLINE bool submit(ThreadTaskItem* item, SteadyClock::TimePoint& time) HX_NOEXCEPT
		{
			LockGuard lck(m_mutex);
			if (m_running)
			{
				m_executing.fetch_add(1, std::memory_order_relaxed);
				auto& entry = m_queue.emplace_back(item);
				time = entry.time;
				if (m_suspend)
				{
					m_cond.notify_one();
				}
				return true;
			}
			item->release();
			return false;
		}

		//初始化当前对象。
		HX_INLINE explicit ThreadLoop(const CString& name = CString{}) HX_NOEXCEPT
			: m_name(name.ptr(), name.size())
		{
			//NOTHING
		}
		//释放当前对象所占用的相关资源。
		~ThreadLoop()
		{
			exit();
			join();
		}
	};
}
using HxThreadLoop = ::HxLibs::ThreadLoop;

#endif //!HX_THREAD_LOOP
#endif //__cplusplus

#endif //!HX_CORE_THREAD_LOOP_H
