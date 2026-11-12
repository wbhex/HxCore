#pragma once
#ifndef HX_CORE_THREAD_TASK_ITEM_H
#define HX_CORE_THREAD_TASK_ITEM_H

#ifndef HX_THREAD_TASK_ITEM
#define HX_THREAD_TASK_ITEM

#include "Interface.h"

HX_INTERFACE_DECLARE(HxThreadTaskItem)
{
	//执行处理。
	HX_METHOD(void, execute)(HX_THIS(HxThreadTaskItem)) HX_NOEXCEPT HX_PURE;
	//释放当前对象。
	HX_METHOD(void, release((HX_THIS(HxThreadTaskItem)) HX_NOEXCEPT HX_PURE;
};

#ifdef __cplusplus
namespace HxLibs
{
	using ThreadTaskItem = ::HxThreadTaskItem;
}
#endif //__cplusplus

#endif //!HX_THREAD_TASK_ITEM

#endif //!HX_CORE_THREAD_TASK_ITEM_H
