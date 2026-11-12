/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include "StringPool.h"
#include <HxCore/Memory.h>
#include <HxCore/String.h>
#include <HxCore/CString.h>
#include <HxCore/LockGuard.h>
#include <HxCore/Containers/HashSet.h>

namespace HxLibs
{
	namespace Current
	{
		template <typename Char = char>
		struct DefaultStringPool : Uncopyable, public XStringPool<Char>
		{
			using HashSet = ::HxLibs::HashSet<XCString<Char>>;
			using LockGuard = ::HxLibs::LockGuard<AtomicLocker>;

		private:
			AtomicLocker m_locker;
			HashSet* m_hashSet{ nullptr };

			static const Char* _empty;

			HX_INLINE static XCString<Char> make(const XCString<Char>& value) HX_NOEXCEPT
			{
				auto bytes = (value.size() + 1) * sizeof(Char);
				auto buffer = Memory::allocAligned(bytes, sizeof(void*));
				memcpy(buffer, value.ptr(), bytes);
				return XCString<Char>((const Char*)buffer, value.size());
			}

		public:
			virtual XCString<Char> HX_STDCALL retrieve(const XCString<Char>* value) HX_NOEXCEPT override
			{
				//空值处理
				if (value == nullptr || value->empty())
				{
					return XCString(_empty, 0);
				}
				//首次检查
				LockGuard lck(m_locker);
				if (m_hashSet != nullptr)
				{
					auto itr = m_hashSet->find(*value);
					if (itr != m_hashSet->end())
					{
						return *itr;
					}
				}
				else
				{
					m_hashSet = Memory::makeObject<HashSet>(2048);
				}
				//新建处理
				lck.unlock();
				XCString<Char> ret = make(*value);
				lck.lock();
				auto itr = m_hashSet->find(*value);
				if (itr != m_hashSet->end())
				{//已经缓存则释放新建并返回
					Memory::freeAligned((void*)ret.ptr());
					return *itr;
				}
				else
				{//缓存并返回
					m_hashSet->insert(ret);
					return ret;
				}
			}

			HX_INLINE DefaultStringPool() HX_NOEXCEPT
			{
				//NOTHING
			}

			~DefaultStringPool()
			{
				if (m_hashSet != nullptr)
				{
					for (auto itr = m_hashSet->begin(); itr != m_hashSet->end(); ++itr)
					{
						Memory::freeAligned((void*)(itr->ptr()));
					}
					Memory::freeObject(m_hashSet);
					m_hashSet = nullptr;
				}
			}
		};

		template <> const char* DefaultStringPool<char>::_empty = "";
		template <> const wchar_t* DefaultStringPool<wchar_t>::_empty = L"";

		static DefaultStringPool<char> g_defaultStringPool{};
		static DefaultStringPool<wchar_t> g_defaultWStringPool{};

		HxStringPool* HX_STDCALL defaultStringPool() HX_NOEXCEPT
		{
			return &g_defaultStringPool;
		}

		HxWStringPool* HX_STDCALL defaultWStringPool() HX_NOEXCEPT
		{
			return &g_defaultWStringPool;
		}
	}
}
