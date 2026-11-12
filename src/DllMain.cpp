/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#include <HxCore/Defines.h>
#define HX_CORE_API HX_API_EXPORT

#include <HxCore/API.h>
#include <HxCore/Memory.h>
#include <HxCore/LockGuard.h>
#include <HxCore/FunctionManager.h>
#include <HxCore/InterfaceManager.h>

#include "Memory.h"
#include <HxCore/MemoryGlobalDefault.h>
#include <HxCore/MemoryGlobalAligned.h>

#include "StringPool.h"
#include <HxCore/StringPoolDefault.h>

namespace HxLibs
{
	namespace Current
	{
		class FunctionManager : public ::HxLibs::FunctionManager
		{
		public:
			HX_INLINE FunctionManager() HX_NOEXCEPT
			{
				add(Uuid(HX_MEMORY_GLOBAL_RESOURCE_DEFAULT_FUNC), (void*)&globalMemoryResourceDefault);
				add(Uuid(HX_MEMORY_GLOBAL_RESOURCE_ALIGNED_FUNC), (void*)&globalMemoryResourceAligned);

				add(Uuid(HX_MEMORY_GLOBAL_ALLOC_PERF_MONITORING_FUNC), (void*)&globalMemoryAllocPerfMonitoring);

				add(Uuid(HX_STRING_POOL_DEFAULT_FUNC), (void*)&defaultStringPool);
				add(Uuid(HX_WSTRING_POOL_DEFAULT_FUNC), (void*)&defaultWStringPool);
			}
		};

		class InterfaceManager : public ::HxLibs::InterfaceManager
		{
		public:
			HX_INLINE explicit InterfaceManager(FunctionManager* functions) HX_NOEXCEPT
				: ::HxLibs::InterfaceManager(true)
			{
				add(static_cast<HxFunctionRegistry*>(functions));
				add(static_cast<HxFunctionEnumPrincipal*>(functions));
			}
		};

		class Manager : Uncopyable
		{
			using LockGuard = ::HxLibs::LockGuard<AtomicLocker>;

		private:
			AtomicLocker m_locker;
			FunctionManager* m_functions{ nullptr };
			InterfaceManager* m_interfaces{ nullptr };

			HX_INLINE void init() HX_NOEXCEPT
			{
				if (m_interfaces == nullptr)
				{
					LockGuard lck(m_locker);
					if (m_interfaces == nullptr)
					{
						m_functions = Memory::makeObject<FunctionManager>();
						m_interfaces = Memory::makeObject<InterfaceManager>(m_functions);
					}
				}
			}

		public:
			HX_INLINE void* queryFunction(const Uuid* uuid) HX_NOEXCEPT
			{
				init();
				return m_functions->query(uuid);
			}
			HX_INLINE void* queryInterface(const Uuid* uuid) HX_NOEXCEPT
			{
				init();
				return m_interfaces->query(uuid);
			}

			HX_INLINE Manager() HX_NOEXCEPT
			{
				//NOTHING
			}
			~Manager()
			{
				if (m_interfaces != nullptr)
				{
					Memory::freeObject(m_interfaces);
					m_interfaces = nullptr;
					Memory::freeObject(m_functions);
					m_functions = nullptr;
				}
			}
		};
		static Manager g_manager{};
	}
}

HX_CORE_API void* HX_STDCALL HxCore_fquery(const HxUuid* uuid) HX_NOEXCEPT
{
	return ::HxLibs::Current::g_manager.queryFunction(uuid);
}

HX_CORE_API void* HX_STDCALL HxCore_iquery(const HxUuid* uuid) HX_NOEXCEPT
{
	return ::HxLibs::Current::g_manager.queryInterface(uuid);
}

#if HX_WINDOWS
#include <Windows.h>
#include <filesystem>
::std::string g_dllPath; 
HX_INLINE void __initDllPath__(HMODULE hmodule) HX_NOEXCEPT
{
	char fileName[0x400];
	GetModuleFileName(hmodule, fileName, sizeof(fileName));
	::std::filesystem::path currentLibPath(fileName);
	currentLibPath = currentLibPath.parent_path().lexically_normal();
	g_dllPath = currentLibPath.string();
}
//DLL入口函数。
BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_DETACH:
		break;
	case DLL_PROCESS_ATTACH:
		__initDllPath__(hmodule);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
#endif //HX_WINDOWS
