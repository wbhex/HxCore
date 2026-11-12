/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_DLL_MODULE_H
#define HX_CORE_DLL_MODULE_H

#ifdef __cplusplus
#ifndef HX_DLL_MODULE
#define HX_DLL_MODULE

#include <type_traits>
#include "Uncopyable.h"

#if HX_WINDOWS
#include <Windows.h>
#else //!HX_WINDOWS
#include <dlfcn.h>
#endif //HX_WINDOWS
#include "String.h"

namespace HxLibs
{
	//表示动态库模块。
	class DllModule : Uncopyable
	{
	public:
#if HX_WINDOWS
		typedef HMODULE Handle;
		HX_INLINE static HMODULE load(const char* path) HX_NOEXCEPT
		{
			return LoadLibraryExA(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		}
		HX_INLINE static HMODULE load(const char* path, int flags) HX_NOEXCEPT
		{
			return LoadLibraryExA(path, NULL, (unsigned int)flags);
		}
		HX_INLINE static void unload(HMODULE handle) HX_NOEXCEPT
		{
			FreeLibrary(handle);
		}
		HX_INLINE static void* resolve(HMODULE handle, const char* name) HX_NOEXCEPT
		{
			return GetProcAddress(handle, name);
		}
		template <typename Traits, typename Alloc>
		HX_INLINE static bool error(XString<char, Traits, Alloc>& msg) HX_NOEXCEPT
		{
			auto v = GetLastError();
			if (v != 0)
			{
				LPSTR buf = nullptr;
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, v, 0, (LPSTR)&buf, 0, nullptr);
				if (buf != nullptr)
				{
					msg.assign(buf);
					LocalFree(buf);
				}
				else
				{
					char buffer[32];
					auto length = sprintf(buffer, "%d", v);
					msg.assign(buffer, length);
				}
				return true;
			}
			return false;
		}
#else //!HX_WINDOWS
		typedef void* Handle;
		HX_INLINE static void* load(const char* path) HX_NOEXCEPT
		{
			return dlopen(path, RTLD_NOW | RTLD_GLOBAL);
		}
		HX_INLINE static void* load(const char* path, int mode) HX_NOEXCEPT
		{
			return dlopen(path, mode);
		}
		HX_INLINE static void unload(void* handle) HX_NOEXCEPT
		{
			dlclose(handle);
		}
		HX_INLINE static void* resolve(void* handle, const char* name) HX_NOEXCEPT
		{
			return dlsym(handle, name);
		}
		template <typename Traits, typename Alloc>
		HX_INLINE static bool error(XString<char, Traits, Alloc>& msg) HX_NOEXCEPT
		{
			const char* v = dlerror();
			if (v != nullptr)
			{
				msg = v;
				return true;
			}
			return false;
		}
#endif //HX_WINDOWS

		//获取错误消息
		template <typename Traits = CharDefaultTraits<char>, typename Alloc = StringDefaultAllocator<char>>
		HX_INLINE static XString<char, Traits, Alloc> error() HX_NOEXCEPT
		{
			XString<char, Traits, Alloc> msg;
			DllModule::error(msg);
			return msg;
		}

	private:
		Handle m_handle;

	public:
		//创建动态库对象。
		HX_INLINE DllModule() HX_NOEXCEPT
			: m_handle(nullptr)
		{
			//NOTHING
		}

		//创建动态库对象，并加载动态连接库。
		HX_INLINE explicit DllModule(const char* path) HX_NOEXCEPT
			: m_handle(load(path))
		{
			//NOTHING
		}
		//创建动态库对象，并加载动态连接库。
		HX_INLINE DllModule(const char* path, int mode) HX_NOEXCEPT
			: m_handle(load(path, mode))
		{
			//NOTHING
		}
		//创建动态库对象，并加载动态连接库。
		template <typename Traits, typename Alloc>
		HX_INLINE explicit DllModule(const XString<char, Traits, Alloc>& path) HX_NOEXCEPT
			: m_handle(load(path.c_str()))
		{
			//NOTHING
		}
		//创建动态库对象，并加载动态连接库。
		template <typename Traits, typename Alloc>
		HX_INLINE DllModule(const XString<char, Traits, Alloc>& path, int mode) HX_NOEXCEPT
			: m_handle(load(path.c_str(), mode))
		{
			//NOTHING
		}

		//创建动态库对象，并加载动态连接库。
		template <typename Traits, typename Alloc>
		HX_INLINE DllModule(const char* path, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
			: m_handle(load(path))
		{
			error.clear();
			if (m_handle == nullptr)
			{
				DllModule::error(error);
			}
		}
		//创建动态库对象，并加载动态连接库。
		template <typename Traits, typename Alloc>
		HX_INLINE DllModule(const char* path, int mode, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
			: m_handle(load(path, mode))
		{
			error.clear();
			if (m_handle == nullptr)
			{
				DllModule::error(error);
			}
		}
		//创建动态库对象，并加载动态连接库。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE DllModule(const XString<char, Traits0, Alloc0>& path, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
			: m_handle(load(path.c_str()))
		{
			error.clear();
			if (m_handle == nullptr)
			{
				DllModule::error(error);
			}
		}
		//创建动态库对象，并加载动态连接库。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE DllModule(const XString<char, Traits0, Alloc0>& path, int mode, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
			: m_handle(load(path.c_str(), mode))
		{
			error.clear();
			if (m_handle == nullptr)
			{
				DllModule::error(error);
			}
		}

		//释放动态连接库。
		HX_INLINE ~DllModule() HX_NOEXCEPT
		{
			if (m_handle != nullptr)
			{
				unload(m_handle);
				m_handle = nullptr;
			}
		}

		//移动构造函数。
		HX_INLINE DllModule(DllModule&& other) HX_NOEXCEPT
			: m_handle(other.m_handle)
		{
			other.m_handle = nullptr;
		}
		//移动操作符。
		HX_INLINE DllModule& operator=(DllModule&& other) HX_NOEXCEPT
		{
			if (this != &other)
			{
				if (m_handle != nullptr)
				{
					unload(m_handle);
				}
				m_handle = other.m_handle;
				other.m_handle = nullptr;
			}
			return *this;
		}

		//执行交换处理。
		HX_INLINE void swap(DllModule& other) HX_NOEXCEPT
		{
			::std::swap(m_handle, other.m_handle);
		}
		//执行交换处理。
		HX_INLINE static void swap(DllModule& a, DllModule& b) HX_NOEXCEPT
		{
			::std::swap(a.m_handle, b.m_handle);
		}

		//重置动态库，执行卸载操作。
		HX_INLINE void reset() HX_NOEXCEPT
		{
			if (m_handle != nullptr)
			{
				unload(m_handle);
				m_handle = nullptr;
			}
		}

		//重置动态库，执行重新加载操作。
		HX_INLINE void reset(const char* path) HX_NOEXCEPT
		{
			auto handle = load(path);
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		HX_INLINE void reset(const char* path, int mode) HX_NOEXCEPT
		{
			auto handle = load(path, mode);
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		template <typename Traits, typename Alloc>
		HX_INLINE void reset(const XString<char, Traits, Alloc>& path) HX_NOEXCEPT
		{
			auto handle = load(path.c_str());
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		template <typename Traits, typename Alloc>
		HX_INLINE void reset(const XString<char, Traits, Alloc>& path, int mode) HX_NOEXCEPT
		{
			auto handle = load(path.c_str(), mode);
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}

		//重置动态库，执行重新加载操作。
		template <typename Traits, typename Alloc>
		HX_INLINE void reset(const char* path, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
		{
			error.clear();
			auto handle = load(path);
			if (handle == nullptr)
			{
				DllModule::error(error);
			}
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		template <typename Traits, typename Alloc>
		HX_INLINE void reset(const char* path, int mode, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
		{
			error.clear();
			auto handle = load(path, mode);
			if (handle == nullptr)
			{
				DllModule::error(error);
			}
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE void reset(const XString<char, Traits0, Alloc0>& path, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
		{
			error.clear();
			auto handle = load(path.c_str());
			if (handle == nullptr)
			{
				DllModule::error(error);
			}
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}
		//重置动态库，执行重新加载操作。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE void reset(const XString<char, Traits0, Alloc0>& path, int mode, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
		{
			error.clear();
			auto handle = load(path.c_str(), mode);
			if (handle == nullptr)
			{
				DllModule::error(error);
			}
			if (m_handle != nullptr)
			{
				unload(m_handle);
			}
			m_handle = handle;
		}

		//卸载动态库。
		HX_INLINE void unload() HX_NOEXCEPT
		{
			reset();
		}

		//重新加载动态库。
		HX_INLINE bool reload(const char* path) HX_NOEXCEPT
		{
			reset(path);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		HX_INLINE bool reload(const char* path, int mode) HX_NOEXCEPT
		{
			reset(path, mode);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		template <typename Traits, typename Alloc>
		HX_INLINE bool reload(const XString<char, Traits, Alloc>& path) HX_NOEXCEPT
		{
			reset(path);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		template <typename Traits, typename Alloc>
		HX_INLINE bool reload(const XString<char, Traits, Alloc>& path, int mode) HX_NOEXCEPT
		{
			reset(path, mode);
			return m_handle != nullptr;
		}

		//重新加载动态库。
		template <typename Traits, typename Alloc>
		HX_INLINE bool reload(const char* path, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
		{
			reset(path, error);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		template <typename Traits, typename Alloc>
		HX_INLINE bool reload(const char* path, int mode, XString<char, Traits, Alloc>& error) HX_NOEXCEPT
		{
			reset(path, mode, error);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE bool reload(const XString<char, Traits0, Alloc0>& path, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
		{
			reset(path, error);
			return m_handle != nullptr;
		}
		//重新加载动态库。
		template <typename Traits0, typename Alloc0, typename Traits1, typename Alloc1>
		HX_INLINE bool reload(const XString<char, Traits0, Alloc0>& path, int mode, XString<char, Traits1, Alloc1>& error) HX_NOEXCEPT
		{
			reset(path, mode, error);
			return m_handle != nullptr;
		}

		//获取是否有效。
		HX_INLINE bool effect() const HX_NOEXCEPT
		{
			return m_handle != nullptr;
		}

		//定位指定的函数。
		HX_INLINE void* resolve(const char* name) HX_NOEXCEPT
		{
			return resolve(m_handle, name);
		}
		//定位指定的函数。
		template <typename Func>
		HX_INLINE void resolve(const char* name, Func* func) HX_NOEXCEPT
		{
			*func = (Func)resolve(m_handle, name);
		}
		//定位指定的函数。
		template <typename Traits, typename Alloc>
		HX_INLINE void* resolve(const XString<char, Traits, Alloc>& name) HX_NOEXCEPT
		{
			return resolve(m_handle, name.c_str());
		}
		//定位指定的函数。
		template <typename Func, typename Traits, typename Alloc>
		HX_INLINE void resolve(const XString<char, Traits, Alloc>& name, Func* func) HX_NOEXCEPT
		{
			*func = (Func)resolve(m_handle, name.c_str());
		}
	};
}
using HxDllModule = ::HxLibs::DllModule;

namespace std
{
	template <>
	HX_INLINE void swap(::HxLibs::DllModule& a, ::HxLibs::DllModule& b) HX_NOEXCEPT
	{
		a.swap(b);
	}
}

#endif //HX_DLL_MODULE
#endif //__cplusplus

#endif //!HX_CORE_DLL_MODULE_H
