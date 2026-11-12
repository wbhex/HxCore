/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CORE_MEMORY_ALLOC_PERF_MONITORING_H
#define HX_CORE_MEMORY_ALLOC_PERF_MONITORING_H

#include "API.h"
#include "Struct.h"
#include "Interface.h"

#ifndef HX_MEMORY_ALLOC_PERF_MONITORING
 //{E3A7E570-094C-433A-9FBF-94E50B05FFC9}
#define HX_MEMORY_ALLOC_PERF_MONITORING 0xe3a7e570, 0x94c, 0x433a, 0x9f, 0xbf, 0x94, 0xe5, 0xb, 0x5, 0xff, 0xc9

 //{EFE53A9A-1F73-4B58-BAF4-F44A621A50FA}
#define HX_MEMORY_GLOBAL_ALLOC_PERF_MONITORING_FUNC 0xefe53a9a, 0x1f73, 0x4b58, 0xba, 0xf4, 0xf4, 0x4a, 0x62, 0x1a, 0x50, 0xfa
HX_INTERFACE_FORWARD_DECLARE(HxMemoryAllocPerfMonitoring);
typedef HX_INTERFACE_FORWARD(HxMemoryAllocPerfMonitoring)* (HX_STDCALL* HxMemoryGlobalAllocPerfMonitoringFunc)();

//表示内存分配性能条目
HX_LIBS_STRUCT_DECLARE(MemoryAllocPerfEntry)
{
	//分配次数
	unsigned long long counter;
	//分配最大耗时纳秒
	unsigned long long maxNanos;
	//分配总共耗时纳秒
	unsigned long long totalNanos;
	//分配最大字节数
	size_t maxBytes;
	//分配上限字节数（0表示无上限）
	size_t limitBytes;
	//分配总共字节数
	unsigned long long totalBytes;
};

//内存分配耗时时长处理函数：以毫秒为单位
typedef void (HX_STDCALL* HxMemoryAllocPerfCallback)(size_t counter, const HxMemoryAllocPerfEntry* entries, size_t entryCount);

//表示内存分配性能监视接口
HX_INTERFACE_DECLARE_EX(HxMemoryAllocPerfMonitoring, HX_MEMORY_ALLOC_PERF_MONITORING)
{
	//设置启用状态
	HX_METHOD(void, setEnabled)(HX_THIS_(HxMemoryAllocPerfMonitoring) bool) HX_NOEXCEPT HX_PURE;
	//刷新结果并重新统计
	HX_METHOD(void, flushResult)(HX_THIS_(HxMemoryAllocPerfMonitoring) HxMemoryAllocPerfCallback) HX_NOEXCEPT HX_PURE;

#ifdef __cplusplus
	//获取全局内存分配性能监视对象
	HX_INLINE static HxMemoryAllocPerfMonitoring* global() HX_NOEXCEPT
	{
		auto func = ::HxLibs::queryFunction<HxMemoryGlobalAllocPerfMonitoringFunc>(HxUuid(HX_MEMORY_GLOBAL_ALLOC_PERF_MONITORING_FUNC));
		return func();
	}
#endif //__cplusplus
};

#ifdef __cplusplus
namespace HxLibs
{
	using MemoryAllocPerfCallback = HxMemoryAllocPerfCallback;
	using MemoryAllocPerfMonitoring = HxMemoryAllocPerfMonitoring;
}
#endif //__cplusplus

#endif //!HX_MEMORY_ALLOC_PERF_MONITORING

#endif //!HX_CORE_MEMORY_ALLOC_PERF_MONITORING_H
