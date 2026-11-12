/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include <HxCore/Defines.h>
#define HX_CORE_API HX_API_EXPORT

#include <cstdlib>
#include "Memory.h"
#include <HxCore/Clock.h>
#include <HxCore/LockGuard.h>
#include <HxCore/LinkedListObject.h>

#if HX_WINDOWS
#include <string>
#include <HxCore/DllModule.h>

extern ::std::string g_dllPath;

namespace HxLibs
{
	namespace Current
	{
		struct ExMalloc : Uncopyable
		{
		private:
			typedef void* (*Align)(size_t, size_t);
			typedef void* (*Alloc)(size_t);
			typedef void (*Free)(void*);
			typedef void (*Release)();

			DllModule m_lib;
			AtomicLocker m_locker;
			Free m_free{ nullptr };
			Alloc m_alloc{ nullptr };
			Align m_align{ nullptr };
			Release m_release{ nullptr };//释放空闲内存
			volatile bool m_loaded{ false };
			bool m_valid{ false };

			bool load_jemalloc()
			{
				auto path = g_dllPath + "\\libjemalloc.dll";
				m_lib.reset(path.c_str());
				if (m_lib.effect())
				{
					m_lib.resolve("je_aligned_alloc", &m_align);
					m_lib.resolve("je_malloc", &m_alloc);
					m_lib.resolve("je_free", &m_free);

					return m_free != nullptr && m_alloc != nullptr && m_align != nullptr;
				}
				return false;
			}

			bool load_tcmalloc()
			{
				auto path = g_dllPath + "\\libtcmalloc.dll";
				m_lib.reset(path.c_str());
				if (m_lib.effect())
				{
					m_lib.resolve("tc_memalign", &m_align);
					m_lib.resolve("tc_malloc", &m_alloc);
					m_lib.resolve("tc_free", &m_free);
					m_lib.resolve("MallocExtension_ReleaseFreeMemory", &m_release);

					return m_free != nullptr && m_alloc != nullptr && m_align != nullptr;
				}
				return false;
			}

		public:
			HX_INLINE void* alloc(size_t bytes, size_t alignment)
			{
				auto buf = m_align(alignment, bytes);
				if (buf != nullptr)
				{
					return buf;
				}
				if (m_release != nullptr)
				{//尝试清理一次并重新分配
					m_release();
					buf = m_align(alignment, bytes);
				}
				return buf;
			}

			HX_INLINE void* alloc(size_t bytes)
			{
				auto buf = m_alloc(bytes);
				if (buf != nullptr)
				{
					return buf;
				}
				if (m_release != nullptr)
				{//尝试清理一次并重新分配
					m_release();
					buf = m_alloc(bytes);
				}
				return buf;
			}

			HX_INLINE void free(void* ptr) HX_NOEXCEPT
			{
				m_free(ptr);
			}

			HX_INLINE bool valid() const HX_NOEXCEPT
			{
				return m_valid;
			}

			bool load() HX_NOEXCEPT
			{
				if (!m_loaded)
				{
					LockGuard<AtomicLocker> lck(m_locker);
					if (!m_loaded)
					{
						if (!load_jemalloc())
						{
							m_valid = load_tcmalloc();
							if (m_valid)
							{
								printf("!!HxCore use libtcmalloc.dll!!\n");
							}
						}
						else
						{
							printf("!!HxCore use libjemalloc.dll!!\n");
							m_valid = true;
						}
						m_loaded = true;
					}
				}
				return m_valid;
			}
		};
		static ExMalloc g_exMalloc{};
	}
}
#endif // HX_WINDOWS

namespace HxLibs
{
	namespace Current
	{
		struct GlobalMemoryAllocPerfDetail;
		struct GlobalMemoryAllocPerfMonitoring;

		//tiny, small, large, huge
		static constexpr size_t GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT = 4;
		static constexpr size_t GLOBAL_MEMORY_ALLOC_PERF_ENTRY_LIMIT_BYTES[GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT] = { 64, 4096, 4 * 1024 * 1024, 0 };

		struct GlobalMemoryAllocPerfDetail : public HxLinkedListNodeBase<GlobalMemoryAllocPerfDetail>
		{
			friend struct GlobalMemoryAllocPerfMonitoring;

		private:
			//条目集合
			MemoryAllocPerfEntry m_entries[GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT];
			//同步对象
			AtomicLocker m_locker;
			using LockGuard = ::HxLibs::SpinLockGuard<AtomicLocker>;
			//是否启用
			bool m_enabled{ false };

			//设置是否启用
			HX_INLINE void enable() HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				m_enabled = true;
			}
			//设置是否启用
			HX_INLINE void disable() HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				m_enabled = false;
				for (size_t i = 0; i < GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT; ++i)
				{
					m_entries[i].counter = 0;
				}
			}

			//将结果刷新到全局中
			HX_INLINE void flush(size_t& counter, MemoryAllocPerfEntry* others) HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				for (size_t i = 0; i < GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT; ++i)
				{
					auto& other = others[i];
					auto& entry = m_entries[i];
					if (entry.counter > 0)
					{
						if (other.counter == 0)
						{
							other.counter = entry.counter;
							other.maxNanos = entry.maxNanos;
							other.totalNanos = entry.totalNanos;
							other.maxBytes = entry.maxBytes;
							other.totalBytes = entry.totalBytes;
						}
						else
						{
							other.counter += entry.counter;
							if (entry.maxNanos > other.maxNanos)
							{
								other.maxNanos = entry.maxNanos;
							}
							other.totalNanos += entry.totalNanos;
							if (entry.maxBytes > other.maxBytes)
							{
								other.maxBytes = entry.maxBytes;
							}
							other.totalBytes += entry.totalBytes;
						}
						counter += entry.counter;
						entry.counter = 0;
					}
				}
			}

		public:
			//更新当前统计值
			HX_INLINE void update(size_t bytes, unsigned long long nanos) HX_NOEXCEPT
			{
				if (m_enabled)
				{
					LockGuard lck(m_locker);
					if (m_enabled)
					{
						size_t idx = GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT - 1;
						for (size_t i = 0; i < GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT; ++i)
						{
							if (bytes <= GLOBAL_MEMORY_ALLOC_PERF_ENTRY_LIMIT_BYTES[i])
							{
								idx = i;
								break;
							}
						}
						auto& entry = m_entries[idx];
						if (entry.counter == 0)
						{
							entry.counter = 1;
							entry.maxNanos = nanos;
							entry.totalNanos = nanos;
							entry.maxBytes = bytes;
							entry.totalBytes = bytes;
						}
						else
						{
							entry.counter += 1;
							if (nanos > entry.maxNanos)
							{
								entry.maxNanos = nanos;
							}
							entry.totalNanos += nanos;
							if (bytes > entry.maxBytes)
							{
								entry.maxBytes = bytes;
							}
							entry.totalBytes += bytes;
						}
					}
				}
			}

			//初始化当前对象
			GlobalMemoryAllocPerfDetail() HX_NOEXCEPT;
			//释放当前对象所占用的相应资源
			~GlobalMemoryAllocPerfDetail();
		};

		struct GlobalMemoryAllocPerfMonitoring : public HxLinkedListObject<GlobalMemoryAllocPerfDetail>, public MemoryAllocPerfMonitoring
		{
			friend struct GlobalMemoryAllocPerfDetail;
			using LockGuard = ::HxLibs::LockGuard<AtomicLocker>;

		private:
			//同步对象
			AtomicLocker m_locker;
			//是否启用
			bool m_enabled{ false };

			//新建一个条目
			HX_INLINE void newItem(GlobalMemoryAllocPerfDetail* item) HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				HxLinkedListObject<GlobalMemoryAllocPerfDetail>::addLast(item);
				item->m_enabled = m_enabled;
			}
			//释放一个条目
			HX_INLINE void freeItem(GlobalMemoryAllocPerfDetail* item) HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				HxLinkedListObject<GlobalMemoryAllocPerfDetail>::remove(item);
			}

		public:
			//是否有效
			HX_INLINE bool effect() const HX_NOEXCEPT
			{
				return m_enabled;
			}

			//构造函数
			HX_INLINE GlobalMemoryAllocPerfMonitoring() HX_NOEXCEPT
			{
				//NOTHING
			}
			//析构函数
			~GlobalMemoryAllocPerfMonitoring()
			{
				//NOTHING
			}

			//设置是否启用
			virtual void HX_STDCALL setEnabled(bool value) HX_NOEXCEPT
			{
				LockGuard lck(m_locker);
				if (value != m_enabled)
				{
					auto item = first();
					if (value)
					{
						while (item != nullptr)
						{
							item->enable();
							item = item->next();
						}
					}
					else
					{
						while (item != nullptr)
						{
							item->disable();
							item = item->next();
						}
					}
					m_enabled = value;
				}
			}
			//刷新结果并重置
			virtual void HX_STDCALL flushResult(MemoryAllocPerfCallback callback) HX_NOEXCEPT
			{
				size_t counter = 0;
				MemoryAllocPerfEntry entries[GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT];
				for (size_t i = 0; i < GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT; ++i)
				{
					auto& entry = entries[i];
					entry.counter = 0;
					entry.limitBytes = GLOBAL_MEMORY_ALLOC_PERF_ENTRY_LIMIT_BYTES[i];
				}

				LockGuard lck(m_locker);
				auto item = first();
				while (item != nullptr)
				{
					item->flush(counter, entries);
					item = item->next();
				}
				lck.unlock();

				callback(counter, entries, GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT);
			}
		};
		static GlobalMemoryAllocPerfMonitoring g_globalMemoryAllocPerfMonitoring{};

		HX_INLINE GlobalMemoryAllocPerfDetail::GlobalMemoryAllocPerfDetail() HX_NOEXCEPT
		{
			for (size_t i = 0; i < GLOBAL_MEMORY_ALLOC_PERF_ENTRY_COUNT; ++i)
			{
				auto& entry = m_entries[i];
				entry.counter = 0;
				entry.limitBytes = GLOBAL_MEMORY_ALLOC_PERF_ENTRY_LIMIT_BYTES[i];
			}
			g_globalMemoryAllocPerfMonitoring.newItem(this);
		}
		HX_INLINE GlobalMemoryAllocPerfDetail::~GlobalMemoryAllocPerfDetail()
		{
			g_globalMemoryAllocPerfMonitoring.freeItem(this);
		}
		static thread_local GlobalMemoryAllocPerfDetail s_globalMemoryAllocPerfDetail{};

		struct GlobalMemoryResourceDefault : public MemoryResource
		{
			//分配的具体实现
			HX_INLINE static void* alloc_impl(size_t size) HX_NOEXCEPT
			{
#if HX_WINDOWS
				auto& exMalloc = g_exMalloc;
				if (exMalloc.load())
				{
					return exMalloc.alloc(size);
				}
				else
				{
					return ::malloc(size);
				}
#else //!HX_WINDOWS
				return ::malloc(size);
#endif //HX_WINDOWS
			}

			//分配操作
			HX_INLINE static void* alloc(size_t size) HX_NOEXCEPT
			{
				if (g_globalMemoryAllocPerfMonitoring.effect())
				{
					auto begin = SteadyClock::now();
					void* buffer = alloc_impl(size);
					s_globalMemoryAllocPerfDetail.update(size, SteadyClock::elapsedNanoseconds(begin));
					return buffer;
				}
				else
				{
					return alloc_impl(size);
				}
			}
			//释放操作
			HX_INLINE static void free(void* ptr) HX_NOEXCEPT
			{
#if HX_WINDOWS
				auto& exMalloc = g_exMalloc;
				if (exMalloc.load())
				{
					exMalloc.free(ptr);
				}
				else
				{
					::free(ptr);
				}
#else //!HX_WINDOWS
				::free(ptr);
#endif // HX_WINDOWS
			}

			//分配指定大小的内存
			virtual void* HX_STDCALL allocate(size_t bytes, size_t align) HX_NOEXCEPT override
			{
				return alloc(bytes);
			}
			//释放之前分配的内存
			virtual void HX_STDCALL deallocate(void* ptr, size_t bytes, size_t align) HX_NOEXCEPT override
			{
				free(ptr);
			}
		};
		static GlobalMemoryResourceDefault g_globalMemoryResourceDefault{};

		struct GlobalMemoryResourceAligned : public MemoryResource
		{
			//分配的具体实现
			HX_INLINE static void* alloc_impl(size_t size, size_t alignment) HX_NOEXCEPT
			{
#if HX_WINDOWS
				auto& exMalloc = g_exMalloc;
				if (exMalloc.load())
				{
					return exMalloc.alloc(size, alignment);
				}
				else
				{
					return ::_aligned_malloc(size, alignment);
				}
#else //!HX_WINDOWS
				return ::std::aligned_alloc(alignment, size);
#endif // HX_WINDOWS
			}

			//分配操作
			HX_INLINE static void* alloc(size_t size, size_t alignment) HX_NOEXCEPT
			{
				if (g_globalMemoryAllocPerfMonitoring.effect())
				{
					auto begin = SteadyClock::now();
					void* buffer = alloc_impl(size, alignment);
					s_globalMemoryAllocPerfDetail.update(size, SteadyClock::elapsedNanoseconds(begin));
					return buffer;
				}
				else
				{
					return alloc_impl(size, alignment);
				}
			}
			//释放操作
			HX_INLINE static void free(void* ptr) HX_NOEXCEPT
			{
#if HX_WINDOWS
				auto& exMalloc = g_exMalloc;
				if (exMalloc.load())
				{
					exMalloc.free(ptr);
				}
				else
				{
					::_aligned_free(ptr);
				}
#else //!HX_WINDOWS
				::free(ptr);
#endif // HX_WINDOWS
			}

			//分配指定大小的内存
			virtual void* HX_STDCALL allocate(size_t bytes, size_t align) HX_NOEXCEPT override
			{
				return alloc(bytes, align);
			}
			//释放之前分配的内存
			virtual void HX_STDCALL deallocate(void* ptr, size_t bytes, size_t align) HX_NOEXCEPT override
			{
				free(ptr);
			}
		};
		static GlobalMemoryResourceAligned g_globalMemoryResourceAligned{};

		MemoryResource* HX_STDCALL globalMemoryResourceDefault() HX_NOEXCEPT
		{
			return &g_globalMemoryResourceDefault;
		}

		MemoryResource* HX_STDCALL globalMemoryResourceAligned() HX_NOEXCEPT
		{
			return &g_globalMemoryResourceAligned;
		}

		MemoryAllocPerfMonitoring* HX_STDCALL globalMemoryAllocPerfMonitoring() HX_NOEXCEPT
		{
			return &g_globalMemoryAllocPerfMonitoring;
		}
	}
}


HX_CORE_API void* HX_STDCALL HxCore_malloc(size_t size) HX_NOEXCEPT
{
	return HxLibs::Current::GlobalMemoryResourceDefault::alloc(size);
}

HX_CORE_API void HX_STDCALL HxCore_mfree(void* ptr) HX_NOEXCEPT
{
	HxLibs::Current::GlobalMemoryResourceDefault::free(ptr);
}


HX_CORE_API void* HX_STDCALL HxCore_malloc_align(size_t size, size_t align) HX_NOEXCEPT
{
	return HxLibs::Current::GlobalMemoryResourceAligned::alloc(size, align);
}

HX_CORE_API void HX_STDCALL HxCore_mfree_align(void* ptr) HX_NOEXCEPT
{
	HxLibs::Current::GlobalMemoryResourceAligned::free(ptr);
}
