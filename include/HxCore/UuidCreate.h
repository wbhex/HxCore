/*
 * Author: HeXiong
 * Mail: info@hexiong.cn
 * Copyright (C) HeXiong & HxSoft Team
 *
 * 本代码受软件著作权保护，不得删除版权声明。
 */

#pragma once
#ifndef HX_CREATE_UUID_CREATE_H
#define HX_CREATE_UUID_CREATE_H

#ifndef HX_UUID_CREATE
#define HX_UUID_CREATE

#include "Uuid.h"

#if HX_WINDOWS && defined(HX_UUID_WINRPC)
#include <rpc.h>
#pragma comment(lib,"Rpcrt4.lib")
#else //!HX_WINDOWS && HX_UUID_WINRPC
#ifdef __cplusplus
#include <chrono>
#include <random>
HX_INLINE int HxUuid_rand32Value() HX_NOEXCEPT
{
	unsigned int seed = (unsigned int)::std::chrono::system_clock::now().time_since_epoch().count();
	::std::default_random_engine generator(seed);
	::std::uniform_int_distribution<int> distribution;
	return ((distribution(generator) & 0x3) << 30) | ((distribution(generator) & 0x7fff) << 15) | (distribution(generator) & 0x7fff);
}
#else //!__cplusplus
#include <stdio.h>
#include <stdlib.h>
HX_INLINE int HxUuid_rand32Value() HX_NOEXCEPT
{
	return ((rand() & 0x3) << 30) | ((rand() & 0x7fff) << 15) | (rand() & 0x7fff);
}
#endif //! __cplusplus
#endif //!HX_WINDOWS && HX_UUID_WINRPC

//创建一个新的UUID。
HX_INLINE void HxUuid_create(HxUuid* v) HX_NOEXCEPT
{
#if HX_WINDOWS && defined(HX_UUID_WINRPC)
	UuidCreate((UUID*)v);
#else //!HX_WINDOWS && HX_UUID_WINRPC
	v->data1 = HxUuid_rand32Value();
	v->data2 = HxUuid_rand32Value() & 0xffff;
	v->data3 = ((HxUuid_rand32Value() & 0x0fff) | 0x4000);
	*((short*)&v->data4[0]) = (HxUuid_rand32Value() & 0x3fff) + 0x8000;
	*((short*)&v->data4[2]) = HxUuid_rand32Value() & 0xffff;
	*((int*)&v->data4[4]) = HxUuid_rand32Value();
#endif //!HX_WINDOWS && HX_UUID_WINRPC
}

#ifdef __cplusplus
HX_INLINE HxUuid HxUuid_create() HX_NOEXCEPT
{
	HxUuid uuid;
	HxUuid_create(&uuid);
	return uuid;
}
HX_INLINE void HxUuid_create(HxUuid& v) HX_NOEXCEPT
{
	HxUuid_create(&v);
}
#endif //! __cplusplus

#endif //!HX_UUID_CREATE

#endif //!HX_CREATE_UUID_CREATE_H
