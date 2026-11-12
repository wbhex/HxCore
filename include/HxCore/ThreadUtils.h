#pragma once
#ifndef HX_CORE_THREAD_UTILS_H
#define HX_CORE_THREAD_UTILS_H

#include "Defines.h"
#if HX_WINDOWS
#include <Windows.h>
#else //!HX_WINDOWS
#include <unistd.h>
#endif //!HX_WINDOWS

//请使用该函数替换掉::std::this_sleep_for函数，标准库早期实现过程中使用了绝对时间
HX_INLINE void HxThread_sleep(unsigned int milliseconds) HX_NOEXCEPT
{
#if HX_WINDOWS
	::Sleep(milliseconds);
#else //!HX_WINDOWS
	::usleep(milliseconds * 1000);
#endif //!HX_WINDOWS
}

HX_INLINE bool HxThread_setName(const char* name, size_t size) HX_NOEXCEPT
{
#if HX_WINDOWS
	HMODULE hModule = ::GetModuleHandleW(L"Kernel32.dll");
	typedef HRESULT(WINAPI* SetThreadDescription)(HANDLE hThread, PCWSTR lpThreadDescription);
	SetThreadDescription func = (SetThreadDescription)::GetProcAddress(hModule, "SetThreadDescription");
	if (func)
	{
		HRESULT result;
		int length = 1024;
		wchar_t buffer[1024] = { 0 };
		length = ::MultiByteToWideChar(CP_ACP, NULL, name, (int)(size == 0 ? strlen(name) : size), buffer, length);
		if (length > 0)
		{
			result = func(GetCurrentThread(), buffer);
			return SUCCEEDED(result);
		}
		return false;
	}
#endif // HX_WINDOWS
	return false;
}

HX_INLINE void HxThread_mmPause() HX_NOEXCEPT
{
#if HX_WINDOWS
	::YieldProcessor();
#endif // HX_WINDOWS
}

#endif //!HX_CORE_THREAD_UTILS_H
